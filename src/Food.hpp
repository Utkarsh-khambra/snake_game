#pragma once
#include "helpers.hpp"
#include <istream>
#include <span>
#include <vector>
enum class FoodType { Trash, Basic, Good, Delectable };
class FoodStorage {
public:
  std::span<Point> data() noexcept;
  void add_food(Point p, FoodType t) noexcept;
  int available() const noexcept { return _store.size(); }

private:
  std::vector<Point> _store{};
  std::vector<FoodType> _types;
};
