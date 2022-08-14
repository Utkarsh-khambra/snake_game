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
  std::span<std::pair<Point, std::string_view>> data() noexcept;
  void move() noexcept;
  void on_keys(char i) noexcept;

private:
  std::vector<std::pair<Point, std::string_view>> _sections;
  std::vector<Direction> _dirs;
};
