#pragma once
#include <string_view>
const std::string_view CSI = "\x9B";
const std::string_view ESC = "\x1B";

struct Point {
  int x = 0;
  int y = 0;
};

struct Rect {
  Point tl; // Top left corner
  int length = 1;
  int height = 1;
};
void switch_raw_mode();