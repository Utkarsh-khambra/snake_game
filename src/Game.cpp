#include "Game.hpp"
#include <assert.h>
// TODO: Linux specific
#include <fmt/core.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>
static int kbhit() {
  timeval tv;
  fd_set rdfs;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&rdfs);
  FD_SET(STDIN_FILENO, &rdfs);

  select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);
}

void SnakeGame::run() {
  switch_raw_mode();
  char key_press = 0;
  fmt::print("Hel");
  while (!kbhit()) {
    auto read_c = read(STDIN_FILENO, &key_press, 1);
    if (key_press == 'q')
      break;
  }
}
