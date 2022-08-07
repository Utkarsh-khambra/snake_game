#pragma once
#include "Screen.hpp"
#include "Snake.hpp"
class SnakeGame {
public:
  SnakeGame() = default;
  void run();
  // starts a new frame
  void update() noexcept;
  void render() noexcept;

private:
  Screen _screen;
};
