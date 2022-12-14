#pragma once

#include "helpers.hpp"
#include <random>
#include <span>
#include <string>
#include <unistd.h>
// Abstracts away the the terminal screen on which game will be rendered
class Screen {
public:
  Screen();
  void render(std::span<std::pair<Point, std::string_view>> vertices,
              std::span<Color> Colors) noexcept;
  void render(std::span<Point> vertices, Color c) const noexcept;
  void clear() const noexcept;
  bool is_on_screen(Point p) const noexcept;
  void show_gameover() const noexcept;
  bool did_hit_borders(Point p) const noexcept;
  Point get_random_point() noexcept;
  float get_aspect_ratio() const noexcept;

private:
  void set_cell_size();
  Point to_screen_space(Point p) const noexcept;
  bool contains(Point p) const noexcept;
  void print_cell_with(Point p, std::string_view fill, Color c) const noexcept;
  int _screen_width = 0;
  int _screen_height = 0;
  int _cells_per_block = 1;
  std::uniform_int_distribution<int> _random_x;
  std::uniform_int_distribution<int> _random_y;
  std::minstd_rand _gen;
};
