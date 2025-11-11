#include "../../tetris.h"

void init_colors() {
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_RED);
  init_pair(2, COLOR_WHITE, COLOR_GREEN);
  init_pair(3, COLOR_WHITE, COLOR_YELLOW);
  init_pair(4, COLOR_WHITE, COLOR_BLUE);
  init_pair(5, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(6, COLOR_WHITE, COLOR_CYAN);
  init_pair(7, COLOR_WHITE, COLOR_WHITE);
  init_pair(8, COLOR_RED, COLOR_BLACK);
  init_pair(9, COLOR_GREEN, COLOR_BLACK);
  init_pair(10, COLOR_BLUE, COLOR_BLACK);
  init_pair(11, COLOR_GREEN, COLOR_WHITE);
}

void draw_game(GameInfo_t *game_info) {
  if (!game_info->screen_cleared) {
    clear();
    game_info->screen_cleared = true;
  }
  draw_field(game_info);
  draw_curr_figure(game_info);
  draw_controls(game_info);
  draw_next_figure(game_info);
}

void draw_controls(GameInfo_t *game_info) {
  draw_hud(FIELD_OFFSET_X - 1, 8, HUD_WIDTH, INFO_HEIGHT);

  mvprintw(8, FIELD_OFFSET_X + 6, "Info");
  mvprintw(9, FIELD_OFFSET_X + 1, "Score ");
  mvprintw(10, FIELD_OFFSET_X + 1, "Highscore ");
  mvprintw(11, FIELD_OFFSET_X + 1, "Level ");

  attron(COLOR_PAIR(9));

  mvprintw(9, FIELD_OFFSET_X + 11, "%d", game_info->score);
  mvprintw(10, FIELD_OFFSET_X + 11, "%d", game_info->max_score);
  mvprintw(11, FIELD_OFFSET_X + 11, "%d", game_info->level);

  mvprintw(14, FIELD_OFFSET_X + 4, "⎵");
  mvprintw(15, FIELD_OFFSET_X + 4, "🡄");
  mvprintw(16, FIELD_OFFSET_X + 4, "🡆");
  mvprintw(17, FIELD_OFFSET_X + 4, "🡇");
  mvprintw(18, FIELD_OFFSET_X + 4, "S");
  mvprintw(19, FIELD_OFFSET_X + 4, "P");
  mvprintw(20, FIELD_OFFSET_X + 4, "Q");

  attroff(COLOR_PAIR(9));

  draw_hud(FIELD_OFFSET_X - 1, 13, HUD_WIDTH, CONTROLS_HEIGHT);

  mvprintw(13, FIELD_OFFSET_X + 4, "Controls");
  mvprintw(14, FIELD_OFFSET_X + 6, "rotate");
  mvprintw(15, FIELD_OFFSET_X + 8, "left");
  mvprintw(16, FIELD_OFFSET_X + 7, "right");
  mvprintw(17, FIELD_OFFSET_X + 8, "down");
  mvprintw(18, FIELD_OFFSET_X + 8, "drop");
  mvprintw(19, FIELD_OFFSET_X + 7, "pause");
  mvprintw(20, FIELD_OFFSET_X + 8, "quit");
}

void draw_field(GameInfo_t *game_info) {
  mvprintw(0, 0, UPPER_LEFT_FIELD_CORNER);
  mvprintw(FIELD_HEIGHT - 1, 0, LOWER_LEFT_FIELD_CORNER);
  mvprintw(0, (FIELD_WIDTH - 1) * 2, UPPER_RIGHT_FIELD_CORNER);
  mvprintw(FIELD_HEIGHT - 1, (FIELD_WIDTH - 1) * 2, LOWER_RIGHT_FIELD_CORNER);

  for (int i = 2; i < FIELD_WIDTH * 2 - 3; ++i) {
    mvprintw(0, i, HORIZONTAL_LINE);
    mvprintw(FIELD_HEIGHT - 1, i, HORIZONTAL_LINE);
  }

  for (int i = 1; i < FIELD_HEIGHT - 1; ++i) {
    mvprintw(i, 0, LEFT_FIELD_BORDER);
    mvprintw(i, (FIELD_WIDTH - 1) * 2, RIGHT_FIELD_BORDER);
  }

  for (int i = 1; i < FIELD_HEIGHT - 1; ++i) {
    for (int j = 1; j < FIELD_WIDTH - 1; ++j) {
      if (game_info->field[i][j])
        draw_figure_block(i, j * 2, game_info->field[i][j]);
      else
        mvprintw(i, j * 2, EMPTY_BLOCK);
    }
  }
  mvprintw(0, FIELD_WIDTH / 2 + 3, "Tetris");
}

void draw_hud(int x, int y, int width, int height) {
  mvprintw(y, x, UPPER_LEFT_HUD_CORNER);
  mvprintw(y, x + width, UPPER_RIGHT_HUD_CORNER);
  mvprintw(y + height, x, LOWER_LEFT_HUD_CORNER);
  mvprintw(y + height, x + width, LOWER_RIGHT_HUD_CORNER);

  for (int i = y + 1; i < y + height; ++i) {
    mvprintw(i, x, HUD_BORDER);
    mvprintw(i, x + width, HUD_BORDER);
  }

  for (int j = x + 1; j < x + width - 1; ++j) {
    mvprintw(y, j, HORIZONTAL_LINE);
    mvprintw(y + height, j, HORIZONTAL_LINE);
  }
}

void draw_curr_figure(GameInfo_t *game_info) {
  for (int i = 0; i < FIGURE_SIZE; ++i) {
    for (int j = 0; j < FIGURE_SIZE; ++j) {
      if (game_info->current[i][j] != 0)
        draw_figure_block(game_info->figure_y_coord + i,
                          (game_info->figure_x_coord + j) * 2,
                          game_info->current[i][j]);
    }
  }
}

void draw_next_figure(GameInfo_t *game_info) {
  draw_hud(FIELD_OFFSET_X - 1, 0, HUD_WIDTH, NEXT_FIGURE_HEIGHT);
  mvprintw(0, FIELD_OFFSET_X + 6, "Next");
  int offset_x = get_figure_width(game_info->next) % 2 ? 5 : 4;
  int offset_y = get_figure_height(game_info->next) > 2 ? 2 : 3;
  if (game_info->next[1][1] == 1) offset_y--;

  for (int i = 1; i < HUD_WIDTH - 1; ++i)
    mvprintw(2, FIELD_OFFSET_X - 1 + i, "  ");
  for (int i = 0; i < FIGURE_SIZE; ++i) {
    for (int j = 0; j < FIGURE_SIZE; ++j) {
      if (game_info->next[i][j])
        draw_figure_block(offset_y + i, FIELD_OFFSET_X + offset_x + j * 2,
                          game_info->next[i][j]);
      else
        mvprintw(offset_y + i, FIELD_OFFSET_X + offset_x + j * 2, "  ");
    }
  }
}

void draw_figure_block(int x_coord, int y_coord, int color) {
  attron(COLOR_PAIR(color));
  mvprintw(x_coord, y_coord, EMPTY_BLOCK);
  attroff(COLOR_PAIR(color));
}

void draw_start_screen() {
  attron(COLOR_PAIR(10));
  mvprintw(0, 0, TETRIS_BY_PICKLEDL);
  mvprintw(FIELD_HEIGHT / 2 + 1, 0, PRESS_SPACE_TO_START);
  attroff(COLOR_PAIR(10));
}

void draw_game_over_screen(GameInfo_t *game_info) {
  attron(COLOR_PAIR(8));
  mvprintw(FIELD_HEIGHT / 2 - 2, FIELD_WIDTH - 5, "GAME OVER");
  mvprintw(FIELD_HEIGHT / 2 - 1, FIELD_WIDTH - 6, "SCORE = %d",
           game_info->score);

  mvprintw(FIELD_HEIGHT / 2, FIELD_WIDTH - 9, "PRESS Q TO QUIT OR");
  mvprintw(FIELD_HEIGHT / 2 + 1, FIELD_WIDTH - 10, "SPACE  TO PLAY AGAIN");
  attroff(COLOR_PAIR(8));
}

void draw_pause_screen() {
  attron(COLOR_PAIR(8));
  mvprintw((FIELD_HEIGHT - 1) / 2, FIELD_WIDTH - 3, "PAUSE");
  attroff(COLOR_PAIR(8));
}

void updateCurrentState(GameInfo_t *game_info) {
  switch (game_info->state) {
    case Start:
      draw_start_screen();
      break;
    case Playing:
      draw_game(game_info);
      break;
    case Pause:
      draw_pause_screen();
      break;
    case Game_over:
      draw_game_over_screen(game_info);
      break;
    case Exit:
      endwin();
      break;
  }
}
