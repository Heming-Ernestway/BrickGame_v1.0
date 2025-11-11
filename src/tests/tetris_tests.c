#include "tetris_tests.h"

void setup_game(GameInfo_t *game_info, int figure_x_coord, int figure_y_coord,
                int score, int level) {
  init_game(game_info);
  game_info->figure_x_coord = figure_x_coord;
  game_info->figure_y_coord = figure_y_coord;
  game_info->score = score;
  game_info->level = level;
  game_info->state = Playing;
}

void fill_line(GameInfo_t *game_info, int row) {
  for (int i = 0; i < FIELD_WIDTH; ++i) game_info->field[row][i] = 1;
}

void choose_figure(GameInfo_t *game_info, int index) {
  int figures[7][4][4] = {
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
      {{0, 2, 0, 0}, {0, 2, 0, 0}, {0, 2, 0, 0}, {0, 2, 0, 0}},
      {{3, 0, 0, 0}, {3, 3, 3, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{0, 0, 4, 0}, {4, 4, 4, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{5, 5, 0, 0}, {0, 5, 5, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{0, 6, 6, 0}, {6, 6, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{0, 2, 0, 0}, {2, 2, 2, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};

  for (int i = 0; i < FIGURE_SIZE; ++i) {
    for (int j = 0; j < FIGURE_SIZE; ++j)
      game_info->current[i][j] = figures[index][i][j];
  }
}

START_TEST(test_get_figure_width) {
  int figure[FIGURE_SIZE][FIGURE_SIZE] = {
      {3, 0, 0, 0}, {3, 3, 3, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int result = get_figure_width(figure);
  ck_assert_int_eq(result, 3);
}
END_TEST

START_TEST(test_spawn_current_figure_failure) {
  GameInfo_t game_info;
  init_game(&game_info);
  setup_game(&game_info, 2, 1, 0, 1);
  fill_line(&game_info, 1);
  fill_line(&game_info, 2);
  bool result = spawn_current_figure(&game_info);
  ck_assert(!result);
}
END_TEST

START_TEST(test_spawn_current_figure_success) {
  GameInfo_t game_info;
  init_game(&game_info);
  bool result = spawn_current_figure(&game_info);
  ck_assert(result);
}
END_TEST

START_TEST(test_move_figure_down_blocked) {
  GameInfo_t game_info;
  init_game(&game_info);
  setup_game(&game_info, 5, 4, 0, 1);
  fill_line(&game_info, 6);
  bool result = move_figure_down(&game_info);
  ck_assert(!result);
}
END_TEST

START_TEST(test_move_figure_down_success) {
  GameInfo_t game_info;
  init_game(&game_info);
  bool result = move_figure_down(&game_info);
  ck_assert(result);
}
END_TEST

START_TEST(test_move_figure_horizontal_blocked) {
  GameInfo_t game_info;
  init_game(&game_info);
  setup_game(&game_info, 5, 5, 0, 1);
  choose_figure(&game_info, 1);
  game_info.field[5][7] = 1;
  move_figure_horizontal(&game_info, 1);
  ck_assert_int_eq(game_info.figure_x_coord, 5);
}
END_TEST

START_TEST(test_move_figure_horizontal_success) {
  GameInfo_t game_info;
  init_game(&game_info);
  setup_game(&game_info, 5, 5, 0, 1);
  choose_figure(&game_info, 1);
  move_figure_horizontal(&game_info, 1);
  ck_assert_int_eq(game_info.figure_x_coord, 6);
}
END_TEST

START_TEST(test_rotate_figure_blocked) {
  GameInfo_t game_info;
  setup_game(&game_info, 11, 5, 0, 1);
  choose_figure(&game_info, 1);
  int temp_figure[FIGURE_SIZE][FIGURE_SIZE];
  memcpy(temp_figure, game_info.current, sizeof(temp_figure));
  rotate_figure(&game_info);
  for (int i = 0; i < FIGURE_SIZE; ++i) {
    for (int j = 0; j < FIGURE_SIZE; ++j)
      ck_assert_int_eq(game_info.current[i][j], temp_figure[i][j]);
  }
}
END_TEST

START_TEST(test_rotate_figure_success) {
  GameInfo_t game_info;
  init_game(&game_info);
  setup_game(&game_info, 5, 5, 0, 1);
  choose_figure(&game_info, 1);
  int temp_figure[FIGURE_SIZE][FIGURE_SIZE];
  memcpy(temp_figure, game_info.current, sizeof(temp_figure));
  rotate_figure(&game_info);
  bool result = true;
  for (int i = 0; i < FIGURE_SIZE; ++i) {
    for (int j = 0; j < FIGURE_SIZE && result; ++j)
      if (game_info.current[i][j] != temp_figure[i][j]) result = false;
  }
  ck_assert(!result);
}
END_TEST

START_TEST(test_check_lines_empty) {
  GameInfo_t game_info;
  setup_game(&game_info, 5, 5, 0, 1);
  check_lines(&game_info);
  for (int i = 1; i < FIELD_HEIGHT - 1; ++i) {
    for (int j = 1; j < FIELD_WIDTH - 1; ++j)
      ck_assert_int_eq(game_info.field[i][j], 0);
  }
}
END_TEST

START_TEST(test_check_lines_full1) {
  GameInfo_t game_info;
  init_game(&game_info);
  fill_line(&game_info, 20);
  check_lines(&game_info);
  for (int i = 1; i < FIELD_HEIGHT - 1; ++i) {
    for (int j = 1; j < FIELD_WIDTH - 1; ++j)
      ck_assert_int_eq(game_info.field[i][j], 0);
  }
}
END_TEST

START_TEST(test_check_lines_full2) {
  GameInfo_t game_info;
  init_game(&game_info);
  fill_line(&game_info, 20);
  fill_line(&game_info, 19);
  check_lines(&game_info);
  for (int i = 1; i < FIELD_HEIGHT - 1; ++i) {
    for (int j = 1; j < FIELD_WIDTH - 1; ++j)
      ck_assert_int_eq(game_info.field[i][j], 0);
  }
}
END_TEST

START_TEST(test_check_lines_full3) {
  GameInfo_t game_info;
  init_game(&game_info);
  fill_line(&game_info, 20);
  fill_line(&game_info, 19);
  fill_line(&game_info, 18);
  check_lines(&game_info);
  for (int i = 1; i < FIELD_HEIGHT - 1; ++i) {
    for (int j = 1; j < FIELD_WIDTH - 1; ++j)
      ck_assert_int_eq(game_info.field[i][j], 0);
  }
}
END_TEST

START_TEST(test_check_lines_full) {
  GameInfo_t game_info;
  init_game(&game_info);
  fill_line(&game_info, 20);
  fill_line(&game_info, 19);
  fill_line(&game_info, 18);
  fill_line(&game_info, 17);
  check_lines(&game_info);
  for (int i = 1; i < FIELD_HEIGHT - 1; ++i) {
    for (int j = 1; j < FIELD_WIDTH - 1; ++j)
      ck_assert_int_eq(game_info.field[i][j], 0);
  }
}
END_TEST

START_TEST(test_hard_drop) {
  GameInfo_t game_info;
  setup_game(&game_info, 5, 5, 0, 1);
  choose_figure(&game_info, 6);
  drop_figure(&game_info);
  ck_assert_int_eq(game_info.figure_y_coord, 19);
}
END_TEST

START_TEST(test_fix_figure) {
  GameInfo_t game_info;
  setup_game(&game_info, 5, 19, 0, 1);
  choose_figure(&game_info, 6);
  place_figure(&game_info);
  for (int i = 0; i < FIGURE_SIZE; ++i) {
    for (int j = 0; j < FIGURE_SIZE; ++j) {
      if (game_info.current[i][j] == 1) {
        ck_assert_int_eq(game_info.field[game_info.figure_y_coord + i]
                                        [game_info.figure_x_coord + j],
                         1);
      }
    }
  }
}
END_TEST

START_TEST(test_load_max_score) {
  GameInfo_t game_info;
  init_game(&game_info);
  game_info.max_score = 5000;
  save_max_score(&game_info);
  load_max_score(&game_info);
  ck_assert_int_eq(game_info.max_score, 5000);
}
END_TEST

START_TEST(test_save_max_score) {
  GameInfo_t game_info;
  init_game(&game_info);
  game_info.max_score = 5000;
  save_max_score(&game_info);
  load_max_score(&game_info);
  ck_assert_int_eq(game_info.max_score, 5000);
}
END_TEST

START_TEST(test_userInput_exit) {
  GameInfo_t game_info;
  setup_game(&game_info, 5, 5, 0, 1);
  userInput(&game_info, 'q');
  ck_assert(game_info.state == Exit);
}
END_TEST

START_TEST(test_userInput_pause) {
  GameInfo_t game_info;
  setup_game(&game_info, 5, 5, 0, 1);
  userInput(&game_info, 'p');
  ck_assert(game_info.state == Pause);
}
END_TEST

START_TEST(test_change_speed) {
  GameInfo_t game_info;
  init_game(&game_info);
  setup_game(&game_info, 5, 17, 0, 10);
  choose_figure(&game_info, 1);
  int timer = 10;
  fill_line(&game_info, 1);
  change_speed(&game_info, &timer);
  ck_assert(game_info.state == Game_over);
}
END_TEST

START_TEST(test_change_speed2) {
  GameInfo_t game_info;
  init_game(&game_info);
  setup_game(&game_info, 5, 17, 0, 10);
  choose_figure(&game_info, 1);
  int timer = 10;
  change_speed(&game_info, &timer);
  ck_assert(game_info.figure_placed == false);
}
END_TEST

START_TEST(test_userInput_q) {
  GameInfo_t game_info;
  init_game(&game_info);
  setup_game(&game_info, 5, 5, 1, 1);
  choose_figure(&game_info, 6);
  game_info.state = Playing;
  userInput(&game_info, 'q');
  ck_assert(game_info.state == Exit);
  game_info.state = Playing;
  userInput(&game_info, 'Q');
  ck_assert(game_info.state == Exit);
}
END_TEST

START_TEST(test_userInput_start) {
  GameInfo_t game_info;
  init_game(&game_info);
  setup_game(&game_info, 5, 5, 1, 1);
  choose_figure(&game_info, 6);
  game_info.state = Start;
  userInput(&game_info, ' ');
  ck_assert(game_info.state == Playing);
  game_info.state = Start;
  userInput(&game_info, 'q');
  ck_assert(game_info.state == Exit);
}
END_TEST

START_TEST(test_userInput_gameover) {
  GameInfo_t game_info;
  init_game(&game_info);
  setup_game(&game_info, 5, 5, 1, 1);
  choose_figure(&game_info, 6);
  game_info.state = Game_over;
  userInput(&game_info, ' ');
  ck_assert(game_info.state == Playing);
  game_info.state = Game_over;
  userInput(&game_info, 'q');
  ck_assert(game_info.state == Exit);
}
END_TEST

START_TEST(test_userInput_paused) {
  GameInfo_t game_info;
  init_game(&game_info);
  setup_game(&game_info, 5, 5, 1, 1);
  choose_figure(&game_info, 6);
  game_info.state = Pause;
  userInput(&game_info, 'p');
  ck_assert(game_info.state == Playing);
  game_info.state = Pause;
  userInput(&game_info, 'q');
  ck_assert(game_info.state == Exit);
}
END_TEST

START_TEST(test_userInput_move) {
  GameInfo_t game_info;
  init_game(&game_info);
  setup_game(&game_info, 5, 5, 1, 1);
  choose_figure(&game_info, 6);
  game_info.state = Playing;
  userInput(&game_info, KEY_LEFT);
  ck_assert_int_eq(game_info.figure_x_coord, 4);
  userInput(&game_info, KEY_RIGHT);
  ck_assert_int_eq(game_info.figure_x_coord, 5);
  userInput(&game_info, ' ');
  ck_assert_int_eq(game_info.figure_y_coord, 5);
  userInput(&game_info, ' ');
  userInput(&game_info, ' ');
  userInput(&game_info, ' ');
  userInput(&game_info, 's');
  ck_assert_int_eq(game_info.figure_y_coord, 19);
}
END_TEST

Suite *tetris_suite() {
  Suite *s = suite_create("Tetris");

  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_get_figure_width);
  tcase_add_test(tc_core, test_spawn_current_figure_failure);
  tcase_add_test(tc_core, test_spawn_current_figure_success);
  tcase_add_test(tc_core, test_move_figure_down_blocked);
  tcase_add_test(tc_core, test_move_figure_down_success);
  tcase_add_test(tc_core, test_move_figure_horizontal_blocked);
  tcase_add_test(tc_core, test_move_figure_horizontal_success);
  tcase_add_test(tc_core, test_rotate_figure_blocked);
  tcase_add_test(tc_core, test_rotate_figure_success);
  tcase_add_test(tc_core, test_check_lines_empty);
  tcase_add_test(tc_core, test_check_lines_full1);
  tcase_add_test(tc_core, test_check_lines_full2);
  tcase_add_test(tc_core, test_check_lines_full3);
  tcase_add_test(tc_core, test_check_lines_full);
  tcase_add_test(tc_core, test_hard_drop);
  tcase_add_test(tc_core, test_fix_figure);
  tcase_add_test(tc_core, test_load_max_score);
  tcase_add_test(tc_core, test_save_max_score);
  tcase_add_test(tc_core, test_userInput_exit);
  tcase_add_test(tc_core, test_userInput_pause);
  tcase_add_test(tc_core, test_change_speed);
  tcase_add_test(tc_core, test_change_speed2);
  tcase_add_test(tc_core, test_userInput_q);
  tcase_add_test(tc_core, test_userInput_start);
  tcase_add_test(tc_core, test_userInput_gameover);
  tcase_add_test(tc_core, test_userInput_paused);
  tcase_add_test(tc_core, test_userInput_move);
  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  Suite *s = tetris_suite();
  SRunner *sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_ENV);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}