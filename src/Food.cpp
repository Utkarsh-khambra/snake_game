#include "Food.hpp"

std::span<Point> FoodStorage::data() noexcept { return _store; }
void FoodStorage::add_food(Point p, FoodType t) noexcept {
  p.props = VertProps::Blink;
  _store.push_back(p);
  _types.push_back(t);
}
