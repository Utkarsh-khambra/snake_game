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
  bool test_collision_with_itself() const noexcept;
  bool test_collision(std::span<Point> targets) const noexcept;
  std::span<Color> colors() noexcept;
  // Vertical to Horizontal speed ratio
  void set_speed_ratio(float ratio) noexcept { _speed_ratio = ratio; }

private:
  std::vector<std::pair<Point, std::string_view>> _sections;
  std::vector<Color> _colrs;
  std::vector<Direction> _dirs;
  float _speed_ratio = 1;
};
