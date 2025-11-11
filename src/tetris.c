#include "tetris.h"

int main() {
  setlocale(LC_ALL, "");
  init_ncurses();
  init_colors();
  game_loop();

  endwin();
  return 0;
}