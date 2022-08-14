#pragma once
#include <string_view>
const std::string_view CSI = "\x9B";
const std::string_view ESC = "\x1B";

enum class VertProps { Blink, None };

struct Point {
  int x = 0;
  int y = 0;
  VertProps props = VertProps::None;
};

struct Rect {
  Point tl; // Top left corner
  int length = 1;
  int height = 1;
};
void switch_raw_mode();
void swith_to_alternate_buffer() noexcept;
void swith_to_normal_buffer() noexcept;
void get_cell_dims() noexcept;
void hide_cursor() noexcept;
void show_cursor() noexcept;
