#pragma once
#include "Food.hpp"
#include "Screen.hpp"
#include "Snake.hpp"
#include <mutex>
#include <queue>
#include <random>
class SnakeGame {
public:
  SnakeGame();
  void run();
  // starts a new frame

private:
  void update() noexcept;
  void handle_input() noexcept;
  void render() noexcept;
  void wait_for_input();
  void generate_food() noexcept;
  Screen _screen;
  Snake _snake;
  std::mutex _input_mutex;
  std::queue<char> _input_q;
  bool _running = true;
  FoodStorage _food;
  std::uniform_int_distribution<int> _random_points;
  std::minstd_rand _gen;
  bool _game_over = false;
};
