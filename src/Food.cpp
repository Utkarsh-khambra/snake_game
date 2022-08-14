#include "Food.hpp"

std::span<Point> FoodStorage::data() noexcept { return _store; }
void FoodStorage::add_food(Point p, FoodType t) noexcept {
  p.props = VertProps::Blink;
  _store[0] = p;
  _store[1] = Point{p.x + 1, p.y, p.props};
  _store[3] = Point{p.x, p.y + 1, p.props};
  _store[2] = Point{p.x + 1, p.y + 1, p.props};
  _t = t;
}
