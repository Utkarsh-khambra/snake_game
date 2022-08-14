#include "helpers.hpp"
#include <fmt/core.h>
#include <termios.h>
#include <unistd.h>
void switch_raw_mode() {
  termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag &= static_cast<unsigned int>(~(ICANON | ECHO));
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
void swith_to_alternate_buffer() noexcept { fmt::print("\033[?1049h"); }
void swith_to_normal_buffer() noexcept { fmt::print("\033[?1049l"); }
void hide_cursor() noexcept { fmt::print("\033[?25l\n"); }
void show_cursor() noexcept { fmt::print("\033[?25h\n"); }

bool test_collistion(Point src, Point target) noexcept {
  return src.x == target.x && src.y == target.y;
}

bool test_collistion_with_line(Point src, Point line_start,
                               Point line_end) noexcept {
  if (line_end.y == line_start.y) {
    return src.y == line_end.y && (src.x < line_end.x && src.x >= line_start.x);
  }
  if (line_end.x == line_start.x) {
    return src.x == line_end.x && (src.y < line_end.y && src.y >= line_start.y);
  }
  return false;
}
void move_cursor_to(int x, int y) noexcept { fmt::print("\033[{};{}H", y, x); }
