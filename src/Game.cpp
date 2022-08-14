#include "Game.hpp"
#include "helpers.hpp"
#include <assert.h>
// TODO: Linux specific
#include <chrono>
#include <fmt/core.h>
#include <functional>
#include <mutex>
#include <termios.h>
#include <thread>
#include <unistd.h>

using namespace std::chrono_literals;

SnakeGame::SnakeGame() {
  auto t = _screen.get_random_point();
  _food.add_food(t, FoodType::Basic);
  _snake.set_speed_ratio(_screen.get_aspect_ratio());
}

static int kbhit() {
  timeval tv;
  fd_set rdfs;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&rdfs);
  FD_SET(STDIN_FILENO, &rdfs);

  select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);
}

void SnakeGame::wait_for_input() {
  char key_press = 0;
  while (_running) {
    kbhit();
    auto read_c = read(STDIN_FILENO, &key_press, 1);
    if (read_c > 0 && _input_q.size() < 4) {
      std::lock_guard<std::mutex> l(_input_mutex);
      _input_q.push(key_press);
    }
  }
}

void SnakeGame::run() {
  switch_raw_mode();
  swith_to_alternate_buffer();
  hide_cursor();
  std::thread t(std::bind_front(&SnakeGame::wait_for_input, this));
  char key_press = 0;
  int count = 1;
  while (_running) {
    auto current = std::chrono::steady_clock::now();
    handle_input();
    update();
    render();
    std::this_thread::sleep_for(
        100ms - std::chrono::duration<double, std::milli>(
                    std::chrono::steady_clock::now() - current));
  }
  t.join();
  show_cursor();
  swith_to_normal_buffer();
}

void SnakeGame::handle_input() noexcept {
  if (_input_mutex.try_lock()) {
    while (!_input_q.empty()) {
      auto i = _input_q.front();
      _input_q.pop();
      if (i == 'q') {
        _running = false;
        _input_mutex.unlock();
        break;
      }
      _snake.on_keys(i);
    }
    _input_mutex.unlock();
  }
}
void SnakeGame::update() noexcept {
  _snake.move();
  if (_snake.test_collision_with_itself()) {
    _running = false;
    _game_over = true;
  }
  if (_snake.test_collision(_food.data())) {
    {
      _snake.push_section();
      _food.add_food(_screen.get_random_point(), FoodType::Basic);
    }
  }
  if (_screen.did_hit_borders(_snake.data().front().first)) {
    _running = false;
    _game_over = true;
  }
}
void SnakeGame::render() noexcept {
  _screen.clear();
  if (_game_over) {
    _screen.show_gameover();
    return;
  }
  _screen.render(_snake.data(), _snake.colors());
  _screen.render(_food.data(), {0, 255, 0});
  fflush(stdout);
}
