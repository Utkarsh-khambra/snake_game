#include "Screen.hpp"
#include "Food.hpp"
#include "helpers.hpp"
#include <array>
#include <cassert>
#include <charconv>
#include <cmath>
#include <cstdio>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <functional>
#include <ranges>
// Linux Specific
#include <sys/ioctl.h>
#include <unistd.h>
#include <utility>

bool Screen::is_on_screen(Point p) const noexcept {
  return contains(to_screen_space(p));
}

void Screen::print_cell_with(Point p, std::string_view fill,
                             Color c) const noexcept {
  move_cursor_to(p.x + 1, p.y + 1);
  // fmt::print("\033[{};{}H", p.y + 1, p.x + 1);
  auto s =
      p.props == VertProps::Blink
          ? fmt::format("\033[38;2;{};{};{}m\033[5m{}", c.r, c.g, c.b, fill)
          : fmt::format("\033[38;2;{};{};{}m{}", c.r, c.g, c.b, fill);
  for (auto i : std::views::iota(0, _cells_per_block))
    fmt::print("{}", s);
  fmt::print("\033[25m");
  fmt::print("\033[39m");
}

Screen::Screen() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  _screen_width = w.ws_col;
  _screen_height = w.ws_row;
  set_cell_size();
  std::random_device rd;
  _gen.seed(rd());
  using p = decltype(_random_x)::param_type;
  _random_x.param(p(0, _screen_width / 2 - FOOD_SIZE));
  _random_y.param(p(0, _screen_height / 2 - FOOD_SIZE));
}

Point Screen::to_screen_space(Point p) const noexcept {
  p.x += _screen_width / 2;
  p.y += _screen_height / 2;
  return p;
}

void Screen::render(std::span<std::pair<Point, std::string_view>> vertices,
                    std::span<Color> colors) noexcept {

  assert(vertices.size() == colors.size());
  for (auto i : std::views::iota(size_t{0}, vertices.size())) {
    if (auto p = to_screen_space(vertices[i].first); contains(p)) {
      print_cell_with(p, vertices[i].second, colors[i]);
    }
  }
}
void Screen::render(std::span<Point> vertices, Color c) const noexcept {

  auto vs =
      vertices |
      std::views::transform(std::bind_front(&Screen::to_screen_space, this)) |
      std::views::filter(std::bind_front(&Screen::contains, this));
  for (auto v : vs)
    print_cell_with(v, "█", c);
}

bool Screen::contains(Point p) const noexcept {
  return p.x >= 0 && p.x < _screen_width && p.y >= 0 && p.y < _screen_height;
}
void Screen::set_cell_size() {
  std::array<char, 20> buffer;
  fmt::print("\033[16t\n");
  auto read_bytes = read(STDIN_FILENO, buffer.data(), buffer.size());
  if (read_bytes < 1)
    throw std::runtime_error("Querying cell sizes is not supported");
  std::string_view useful_info(buffer.data() + 4,
                               static_cast<size_t>(read_bytes));
  auto cell_height = 0;
  auto [ptr, ec] =
      std::from_chars(useful_info.data(),
                      useful_info.data() + useful_info.find(';'), cell_height);
  auto cell_width = 0;
  ++ptr;
  std::from_chars(ptr, ptr + useful_info.find('t'), cell_width);
  _cells_per_block = std::ceil(static_cast<float>(cell_height) / cell_width);
}

void Screen::clear() const noexcept { fmt::print("\033[2J"); }

void Screen::show_gameover() const noexcept {
  move_cursor_to(_screen_width / 2, _screen_height / 2);
  fmt::print("Game over\n");
}

bool Screen::did_hit_borders(Point p) const noexcept {
  p = to_screen_space(p);
  return !(p.x >= 0 && p.x < _screen_width && p.y >= 0 && p.y < _screen_height);
}

Point Screen::get_random_point() noexcept {
  return {_random_x(_gen), _random_y(_gen)};
}

float Screen::get_aspect_ratio() const noexcept {
  return static_cast<float>(_screen_width) / _screen_height;
}
