#include "Snake.hpp"
#include "helpers.hpp"
#include <fmt/core.h>
#include <ranges>

const int SNAKE_FAT = 1;
Snake::Snake() {
  _sections.push_back({{3, 4}, "█"});
  _dirs.push_back(Direction::Left);
}

void Snake::push_section() noexcept {
  auto last = _sections.back();
  auto last_dir = _dirs.back();
  using enum Direction;
  switch (last_dir) {
  case Up:
    last.first.y -= 1;
    last.second = "█";
    break;
  case Down:
    last.first.y += 1;
    last.second = "█";
    break;
  case Left:
    last.first.x += 1;
    last.second = "█";
    break;
  case Right:
    last.first.x -= 1;
    last.second = "█";
    break;
  default:
    break;
  }
  _sections.push_back(last);
  _dirs.push_back(last_dir);
}

void Snake::change_direction_to(Direction d) noexcept { _dirs.front() = d; }
std::span<std::pair<Point, std::string_view>> Snake::data() noexcept {
  return _sections;
}

void Snake::move() noexcept {
  using enum Direction;
  auto prev_dir = _dirs.front();
  // fmt::print("Move\n");
  for (auto i : std::views::iota(size_t(0), _sections.size())) {
    switch (_dirs[i]) {
    case Up:
      _sections[i].first.y += 1;
      _sections[i].second = "█";
      break;
    case Down:
      _sections[i].first.y -= 1;
      _sections[i].second = "█";
      break;
    case Left:
      _sections[i].first.x -= 1;
      _sections[i].second = "█";
      break;
    case Right:
      _sections[i].first.x += 1;
      _sections[i].second = "█";
      break;
    default:
      break;
    }
    std::swap(_dirs[i], prev_dir);
  }
}

void Snake::on_keys(char key) noexcept {
  using enum Direction;
  switch (key) {
  case 'a':
    change_direction_to(Left);
    break;
  case 's':
    change_direction_to(Up);
    break;
  case 'w':
    change_direction_to(Down);
    break;
  case 'd':
    change_direction_to(Right);
    break;
  default:
    break;
  }
}
