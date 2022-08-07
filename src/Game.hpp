#pragma once
#include "Screen.hpp"
#include "Snake.hpp"
#include <mutex>
#include <queue>
class SnakeGame {
public:
  SnakeGame() = default;
  void run();
  // starts a new frame

private:
  void update() noexcept;
  void handle_input() noexcept;
  void render() noexcept;
  void wait_for_input();
  Screen _screen;
  Snake _snake;
  std::mutex _input_mutex;
  std::queue<char> _input_q;
  bool running = true;
};
