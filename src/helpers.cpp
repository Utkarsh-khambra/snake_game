#include "helpers.hpp"
#include <termios.h>
#include <unistd.h>
void switch_raw_mode() {
  termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag &= static_cast<unsigned int>(~(ICANON | ECHO));
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
