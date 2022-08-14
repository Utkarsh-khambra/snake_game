#pragma once
#include "helpers.hpp"
#include <istream>
#include <span>
#include <vector>

constexpr int FOOD_SIZE = 2;
enum class FoodType { Trash, Basic, Good, Delectable };
class FoodStorage {
public:
  std::span<Point> data() noexcept;
  void add_food(Point p, FoodType t) noexcept;

private:
  std::array<Point, FOOD_SIZE * FOOD_SIZE> _store{};
  FoodType _t;
};
