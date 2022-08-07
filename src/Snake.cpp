#include "Snake.hpp"
#include "helpers.hpp"
#include <ranges>

const int SNAKE_FAT = 1;
Snake::Snake() {
	using enum Direction;
  _sections.push_back({3, 4});
  _dirs.push_back(Direction::Left);
  push_section();
  change_direction_to(Down);
	move();
	move();
  push_section();
  change_direction_to(Left);
	move();
	move();
  push_section();
  change_direction_to(Down);
  push_section();
	move();
  push_section();
	move();
}

void Snake::push_section() noexcept {
  auto last = _sections.back();
  auto last_dir = _dirs.back();
  using enum Direction;
  switch (last_dir) {
  case Up:
    last.y -= 1;
    break;
  case Down:
    last.y += 1;
    break;
  case Left:
    last.x += 1;
    break;
  case Right:
    last.x -= 1;
    break;
  default:
    break;
  }
  _sections.push_back(last);
  _dirs.push_back(last_dir);
}

void Snake::change_direction_to(Direction d) noexcept { _dirs.front() = d; }
std::span<Point> Snake::data() noexcept { return _sections; }

void Snake::move() noexcept {
  using enum Direction;
  auto prev_dir = _dirs.front();
  for (auto i : std::views::iota(size_t(0), _sections.size())) {
    switch (_dirs[i]) {
    case Up:
      _sections[i].y += 1;
      break;
    case Down:
      _sections[i].y -= 1;
      break;
    case Left:
      _sections[i].x -= 1;
      break;
    case Right:
      _sections[i].x += 1;
      break;
    default:
      break;
    }
    std::swap(_dirs[i], prev_dir);
  }
}
