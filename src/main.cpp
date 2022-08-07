// #include "Game.hpp"
#include "Screen.hpp"
#include "Snake.hpp"
#include "helpers.hpp"
int main() {
  Snake sn;
  Screen ss;
  ss.render(sn.data());
}
