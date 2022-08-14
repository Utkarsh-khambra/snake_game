#include "Screen.hpp"
#include "helpers.hpp"
#include <array>
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

void Screen::print_cell_with(Point p, std::string_view fill) const noexcept {
  move_cursor_to(p.x + 1, p.y + 1);
  // fmt::print("\033[{};{}H", p.y + 1, p.x + 1);
  auto s = p.props == VertProps::Blink ? fmt::format("\033[5m{}", fill)
                                       : fmt::format("{}", fill);
  for (auto i : std::views::iota(0, _cells_per_block))
    fmt::print("{}", s);
  fmt::print("\033[25m");
}

Screen::Screen() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  _screen_width = w.ws_col;
  _screen_height = w.ws_row;
  set_cell_size();
}

Point Screen::to_screen_space(Point p) const noexcept {
  p.x += _screen_width / 2;
  p.y += _screen_height / 2;
  return p;
}

void Screen::render(
    std::span<std::pair<Point, std::string_view>> vertices) noexcept {

  auto vs = vertices | std::views::transform([this](const auto &v) noexcept {
              return std::make_pair(to_screen_space(v.first), v.second);
            }) |
            std::views::filter(
                [this](const auto &v) noexcept { return contains(v.first); });
  for (auto v : vs)
    print_cell_with(v.first, v.second);
}
void Screen::render(std::span<Point> vertices) const noexcept {

  auto vs =
      vertices |
      std::views::transform(std::bind_front(&Screen::to_screen_space, this)) |
      std::views::filter(std::bind_front(&Screen::contains, this));
  for (auto v : vs)
    print_cell_with(v, "█");
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
  return test_collistion_with_line(p, {0, 0}, {_screen_width, 0}) ||
         test_collistion_with_line(p, {0, 0}, {0, _screen_height}) ||
         test_collistion_with_line(p, {_screen_width, 0},
                                   {_screen_width, _screen_height}) ||
         test_collistion_with_line(p, {0, _screen_height},
                                   {_screen_width, _screen_height});
}
