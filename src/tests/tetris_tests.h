#ifndef TETRISTEST_H
#define TETRISTEST_H

#include <check.h>

#include "../tetris.h"

void choose_figure(GameInfo_t *game_info, int index);
void fill_line(GameInfo_t *game_info, int row);
void setup_game(GameInfo_t *game_info, int figureX, int figureY, int score,
                int level);

#endif