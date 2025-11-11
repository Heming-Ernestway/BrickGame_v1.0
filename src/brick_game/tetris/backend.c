#include "../../tetris.h"

void init_ncurses() {
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  timeout(70);
  mousemask(0, NULL);
  curs_set(0);
}

void init_game(GameInfo_t *game_info) {
  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    for (int j = 0; j < FIELD_WIDTH; ++j) {
      if (i == 0 || j == 0 || j == FIELD_WIDTH - 1 || i == FIELD_HEIGHT - 1)
        game_info->field[i][j] = COLOR_WHITE;
      else
        game_info->field[i][j] = 0;
    }
  }
  game_info->figure_x_coord = FIELD_WIDTH / 2 - FIGURE_SIZE / 2;
  game_info->figure_y_coord = 1;
  game_info->score = 0;
  game_info->level = 1;
  srand(time(NULL));

  generate_next_figure(game_info);
  spawn_current_figure(game_info);
  load_max_score(game_info);
}

void game_loop() {
  GameInfo_t game_info;
  game_info.state = Start;
  game_info.screen_cleared = false;
  int timer = 0;
  int key;
  while (game_info.state != Exit) {
    key = getch();
    userInput(&game_info, key);
    updateCurrentState(&game_info);
    change_speed(&game_info, &timer);
  }
}

void load_max_score(GameInfo_t *game_info) {
  FILE *file = fopen(SCORE_FILE, "r");
  game_info->max_score = 0;
  if (file) {
    fscanf(file, "%d", &game_info->max_score);
    fclose(file);
  } else
    save_max_score(game_info);
}

void save_max_score(GameInfo_t *game_info) {
  FILE *file = fopen(SCORE_FILE, "w");
  if (file) {
    fprintf(file, "%d", game_info->max_score);
    fclose(file);
  } else
    perror("File opening error");
}

void generate_next_figure(GameInfo_t *game_info) {
  int figures[7][4][4] = {
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
      {{0, 2, 0, 0}, {0, 2, 0, 0}, {0, 2, 0, 0}, {0, 2, 0, 0}},
      {{3, 0, 0, 0}, {3, 3, 3, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{0, 0, 4, 0}, {4, 4, 4, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{5, 5, 0, 0}, {0, 5, 5, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{0, 6, 6, 0}, {6, 6, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{0, 2, 0, 0}, {2, 2, 2, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};

  int index = rand() % 7;
  for (int i = 0; i < FIGURE_SIZE; ++i) {
    for (int j = 0; j < FIGURE_SIZE; ++j) {
      game_info->next[i][j] = figures[index][i][j];
    }
  }
}

bool spawn_current_figure(GameInfo_t *game_info) {
  memcpy(game_info->current, game_info->next, sizeof(game_info->current));
  game_info->figure_placed = false;

  int figure_width = get_figure_width(game_info->current);

  game_info->figure_x_coord = FIELD_WIDTH / 2 - figure_width / 2 - 1;
  game_info->figure_y_coord = game_info->current[1][1] == 1 ? 0 : 1;

  for (int i = 0; i < FIGURE_SIZE; ++i) {
    for (int j = 0; j < FIGURE_SIZE; ++j) {
      if (game_info->current[i][j] &&
          game_info->field[game_info->figure_y_coord + i]
                          [game_info->figure_x_coord + j])
        return false;
    }
  }

  generate_next_figure(game_info);
  return true;
}

int get_figure_width(int figure[FIGURE_SIZE][FIGURE_SIZE]) {
  int width = 0;
  for (int i = 0; i < FIGURE_SIZE; ++i) {
    int non_zero_cnt = 0;
    for (int j = 0; j < FIGURE_SIZE && !non_zero_cnt; ++j) {
      if (figure[j][i] != 0) non_zero_cnt++;
    }
    if (non_zero_cnt > 0) width++;
  }
  return width;
}

int get_figure_height(int figure[FIGURE_SIZE][FIGURE_SIZE]) {
  int height = 0;
  for (int i = 0; i < FIGURE_SIZE; ++i) {
    int non_zero_cnt = 0;
    for (int j = 0; j < FIGURE_SIZE && !non_zero_cnt; ++j) {
      if (figure[i][j] != 0) non_zero_cnt++;
    }
    if (non_zero_cnt > 0) height++;
  }
  return height;
}

void place_figure(GameInfo_t *game_info) {
  for (int i = 0; i < FIGURE_SIZE; ++i) {
    for (int j = 0; j < FIGURE_SIZE; ++j) {
      if (game_info->current[i][j]) {
        game_info->field[game_info->figure_y_coord + i]
                        [game_info->figure_x_coord + j] =
            game_info->current[i][j];
      }
    }
  }
}

void check_lines(GameInfo_t *game_info) {
  int lines_cnt = 0;
  for (int i = 1; i < FIELD_HEIGHT - 1; ++i) {
    bool full_line = true;
    for (int j = 1; j < FIELD_WIDTH - 1 && full_line; ++j) {
      if (game_info->field[i][j] == 0) full_line = false;
    }
    if (full_line) {
      lines_cnt++;
      for (int k = i; k > 1; --k) {
        for (int j = 1; j < FIELD_WIDTH - 1; ++j) {
          game_info->field[k][j] = game_info->field[k - 1][j];
        }
      }
    }
  }

  if (lines_cnt) increase_score_and_level(game_info, lines_cnt);

  if (game_info->score > game_info->max_score) {
    game_info->max_score = game_info->score;

    save_max_score(game_info);
  }
}

void increase_score_and_level(GameInfo_t *game_info, int lines_cnt) {
  switch (lines_cnt) {
    case 1:
      game_info->score += ONE_LINE_POINTS;
      break;
    case 2:
      game_info->score += TWO_LINES_POINTS;
      break;
    case 3:
      game_info->score += THREE_LINES_POINTS;
      break;
    case 4:
      game_info->score += FOUR_LINES_POINTS;
      break;
  }

  if (game_info->level < MAX_LEVEL &&
      game_info->score / POINTS_PER_LEVEL > game_info->level - 1)
    game_info->level++;
}

void drop_figure(GameInfo_t *game_info) {
  while (move_figure_down(game_info))
    ;
  ;
}

bool move_figure_down(GameInfo_t *game_info) {
  bool collision = false;

  for (int i = 0; i < FIGURE_SIZE; ++i) {
    for (int j = 0; j < FIGURE_SIZE && !collision; ++j) {
      if (game_info->current[i][j] &&
          game_info->field[game_info->figure_y_coord + i + 1]
                          [game_info->figure_x_coord + j]) {
        collision = true;
      }
    }
  }
  if (!collision) game_info->figure_y_coord++;

  return !collision;
}

void move_figure_horizontal(GameInfo_t *game_info, int dx) {
  bool collision = false;

  for (int i = 0; i < FIGURE_SIZE; ++i) {
    for (int j = 0; j < FIGURE_SIZE && !collision; ++j) {
      if (game_info->current[i][j] &&
          game_info->field[game_info->figure_y_coord + i]
                          [game_info->figure_x_coord + j + dx]) {
        collision = true;
      }
    }
  }

  if (!collision) game_info->figure_x_coord += dx;
}

void rotate_figure(GameInfo_t *game_info) {
  int temp[FIGURE_SIZE][FIGURE_SIZE];
  memcpy(temp, game_info->current, sizeof(temp));

  for (int i = 0; i < FIGURE_SIZE; ++i) {
    for (int j = 0; j < FIGURE_SIZE; ++j) {
      game_info->current[j][FIGURE_SIZE - 1 - i] = temp[i][j];
    }
  }

  bool collision = false;
  for (int i = 0; i < FIGURE_SIZE; ++i) {
    for (int j = 0; j < FIGURE_SIZE && !collision; ++j) {
      if (game_info->current[i][j] &&
          game_info->field[game_info->figure_y_coord + i]
                          [game_info->figure_x_coord + j])
        collision = true;
    }
  }

  if (collision) memcpy(game_info->current, temp, sizeof(temp));
}

void userInput(GameInfo_t *game_info, int key) {
  switch (game_info->state) {
    case Start:
      start_or_exit_game(game_info, key);
      break;
    case Playing:
      user_input_in_game(game_info, key);
      break;
    case Pause:
      if (key == 'p' || key == 'P')
        game_info->state = Playing;
      else if (key == 'q' || key == 'Q')
        game_info->state = Exit;
      break;
    case Game_over:
      start_or_exit_game(game_info, key);
      break;
    default:
      break;
  }
}

void user_input_in_game(GameInfo_t *game_info, int key) {
  switch (key) {
    case 'q':
    case 'Q':
      game_info->state = Exit;
      break;
    case 'p':
    case 'P':
      game_info->state = Pause;
      break;
    case KEY_LEFT:
      move_figure_horizontal(game_info, LEFT_SHIFT);
      break;
    case KEY_RIGHT:
      move_figure_horizontal(game_info, RIGHT_SHIFT);
      break;
    case ' ':
      rotate_figure(game_info);
      break;
    case KEY_DOWN:
      if (!game_info->figure_placed) move_figure_down(game_info);
      break;
    case 's':
    case 'S':
      if (!game_info->figure_placed) drop_figure(game_info);
      break;
  }
}

void start_or_exit_game(GameInfo_t *game_info, int key) {
  if (key == ' ') {
    init_game(game_info);
    game_info->state = Playing;
  } else if (key == 'q' || key == 'Q')
    game_info->state = Exit;
}

void change_speed(GameInfo_t *game_info, int *timer) {
  int delay = 10 - (game_info->level - 1);

  if (game_info->state != Playing || (*timer)++ < delay) return;

  if (!move_figure_down(game_info)) {
    if (!game_info->figure_placed) {
      place_figure(game_info);
      game_info->figure_placed = true;
    }

    check_lines(game_info);

    if (game_info->figure_placed) {
      if (!spawn_current_figure(game_info))
        game_info->state = Game_over;
      else
        game_info->figure_placed = false;
    }
  }

  *timer = 0;
}
