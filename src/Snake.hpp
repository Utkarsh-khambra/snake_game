#pragma once
#include "helpers.hpp"
#include <cmath>
#include <span>
#include <vector>
enum class Direction { Up, Down, Left, Right };
class Snake {
public:
  Snake();
  void change_direction_to(Direction) noexcept;
  void push_section() noexcept;
  std::span<Point> data() noexcept;
  void move() noexcept;

private:
  std::vector<Point> _sections;
  std::vector<Direction> _dirs;
};
