#pragma once

#include "helpers.hpp"
#include <span>
#include <string>
#include <unistd.h>
// Abstracts away the the terminal screen on which game will be rendered
class Screen {
public:
  Screen();
  void render(std::span<Point> vertices) noexcept;
  void clear() noexcept;

private:
  void set_cell_size();
  Point to_screen_space(Point p) const noexcept;
  bool contains(Point p) const noexcept;
  int _screen_width = 0;
  int _screen_height = 0;
  int _cells_per_block = 1;
  std::string _single_cell;
};
