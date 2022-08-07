#include "Screen.hpp"
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

Screen::Screen() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  _screen_width = w.ws_col;
  _screen_height = w.ws_row;
  set_cell_size();
  for (auto i : std::views::iota(0, _cells_per_block))
    _single_cell.append("█");
}

Point Screen::to_screen_space(Point p) const noexcept {
  p.x += _screen_width / 2;
  p.y += _screen_height / 2;
  return p;
}

void Screen::render(std::span<Point> vertices) noexcept {

  auto vs =
      vertices |
      std::views::transform(std::bind_front(&Screen::to_screen_space, this)) |
      std::views::filter(std::bind_front(&Screen::contains, this));
  for (auto v : vs) {
    fmt::print("\033[{};{}H", v.y + 1, v.x + 1);
    fmt::print("{}", _single_cell);
  }
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
