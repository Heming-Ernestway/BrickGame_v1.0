/**
 * @file tetris.h
 */

#ifndef TETRIS_H
#define TETRIS_H

#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FIELD_WIDTH 12
#define FIELD_HEIGHT 22
#define FIGURE_SIZE 4
#define SCORE_FILE "highscore.txt"
#define FIELD_OFFSET_X (FIELD_WIDTH * 2 + 1)
#define LEFT_SHIFT -1
#define RIGHT_SHIFT 1
#define MAX_LEVEL 10
#define POINTS_PER_LEVEL 600
#define ONE_LINE_POINTS 100
#define TWO_LINES_POINTS 300
#define THREE_LINES_POINTS 700
#define FOUR_LINES_POINTS 1500

#define TETRIS_BY_PICKLEDL                                                                                                                             \
  "─────────╔════╗╔═══╗╔════╗╔═══╗╔══╗╔══╗─────────\n" \
  "─────────╚═╗╔═╝║╔══╝╚═╗╔═╝║╔═╗║╚╗╔╝║╔═╝─────────\n" \
  "───────────║║──║╚══╗──║║──║╚═╝║─║║─║╚═╗─────────\n" \
  "───────────║║──║╔══╝──║║──║╔╗╔╝─║║─╚═╗║─────────\n" \
  "───────────║║──║╚══╗──║║──║║║║─╔╝╚╗╔═╝║─────────\n" \
  "───────────╚╝──╚═══╝──╚╝──╚╝╚╝─╚══╝╚══╝─────────\n" \
                                                                                                                                                       \
  "╔══╗─╔╗╔╗──╔═══╗╔══╗╔══╗╔╗╔══╗╔╗──╔═══╗╔══╗─╔╗──\n" \
  "║╔╗║─║║║║──║╔═╗║╚╗╔╝║╔═╝║║║╔═╝║║──║╔══╝║╔╗╚╗║║──\n" \
  "║╚╝╚╗║╚╝║──║╚═╝║─║║─║║──║╚╝║──║║──║╚══╗║║╚╗║║║──\n" \
  "║╔═╗║╚═╗║──║╔══╝─║║─║║──║╔╗║──║║──║╔══╝║║─║║║║──\n" \
  "║╚═╝║─╔╝║──║║───╔╝╚╗║╚═╗║║║╚═╗║╚═╗║╚══╗║╚═╝║║╚═╗\n" \
  "╚═══╝─╚═╝──╚╝───╚══╝╚══╝╚╝╚══╝╚══╝╚═══╝╚═══╝╚══╝"

#define PRESS_SPACE_TO_START                                                                                                                           \
  "╔═══╗╔═══╗╔═══╗╔══╗╔══╗───╔══╗╔═══╗╔══╗╔══╗╔═══╗\n" \
  "║╔═╗║║╔═╗║║╔══╝║╔═╝║╔═╝───║╔═╝║╔═╗║║╔╗║║╔═╝║╔══╝\n" \
  "║╚═╝║║╚═╝║║╚══╗║╚═╗║╚═╗───║╚═╗║╚═╝║║╚╝║║║──║╚══╗\n" \
  "║╔══╝║╔╗╔╝║╔══╝╚═╗║╚═╗║───╚═╗║║╔══╝║╔╗║║║──║╔══╝\n" \
  "║║───║║║║─║╚══╗╔═╝║╔═╝║───╔═╝║║║───║║║║║╚═╗║╚══╗\n" \
  "╚╝───╚╝╚╝─╚═══╝╚══╝╚══╝───╚══╝╚╝───╚╝╚╝╚══╝╚═══╝\n" \
  "─────╔════╗╔══╗───╔══╗╔════╗╔══╗╔═══╗╔════╗─────\n" \
  "─────╚═╗╔═╝║╔╗║───║╔═╝╚═╗╔═╝║╔╗║║╔═╗║╚═╗╔═╝─────\n" \
  "───────║║──║║║║───║╚═╗──║║──║╚╝║║╚═╝║──║║───────\n" \
  "───────║║──║║║║───╚═╗║──║║──║╔╗║║╔╗╔╝──║║───────\n" \
  "───────║║──║╚╝║───╔═╝║──║║──║║║║║║║║───║║───────\n" \
  "───────╚╝──╚══╝───╚══╝──╚╝──╚╝╚╝╚╝╚╝───╚╝───────"
#define UPPER_LEFT_FIELD_CORNER " ┏"
#define LOWER_LEFT_FIELD_CORNER " ┗"
#define UPPER_RIGHT_FIELD_CORNER "┓ "
#define LOWER_RIGHT_FIELD_CORNER "┛ "
#define LEFT_FIELD_BORDER " ┃"
#define RIGHT_FIELD_BORDER "┃ "
#define HORIZONTAL_LINE "━━"
#define HUD_BORDER "┃"
#define UPPER_LEFT_HUD_CORNER "┏"
#define LOWER_LEFT_HUD_CORNER "┗"
#define UPPER_RIGHT_HUD_CORNER "┓"
#define LOWER_RIGHT_HUD_CORNER "┛"
#define HUD_WIDTH 17
#define CONTROLS_HEIGHT 8
#define NEXT_FIGURE_HEIGHT 7
#define INFO_HEIGHT 4
#define EMPTY_BLOCK "  "

/**
 * @brief Состояния игры
 */
typedef enum { Start, Playing, Pause, Game_over, Exit } Game_state;

/**
 * @brief Основная структура игры
 * @param field поле
 * @param current текущая фигура
 * @param next следующая фигура
 * @param figure_x_coord координаты фигуры
 * @param figure_y_coord координаты фигуры
 * @param figure_placed размещена ли фигура на поле
 * @param score счет
 * @param max_score максимальный счет
 * @param level уровень игры
 * @param screen_cleared флаг для очистки экрана
 * @param state состояние игры
 * */
typedef struct {
  int field[FIELD_HEIGHT][FIELD_WIDTH];
  int current[FIGURE_SIZE][FIGURE_SIZE];
  int next[FIGURE_SIZE][FIGURE_SIZE];
  int figure_x_coord;
  int figure_y_coord;
  bool figure_placed;
  int score;
  int max_score;
  int level;
  bool screen_cleared;
  Game_state state;
} GameInfo_t;

/** @brief Функция инициализации ncurses */
void init_ncurses();

/** @brief Функция обновления состояния игры */
void updateCurrentState(GameInfo_t *game_info);

/** @brief Функция инициализации игры */
void init_game(GameInfo_t *game_info);

/** @brief Функция запуска игрового цикла */
void game_loop();

/** @brief Функция инициализации цветов */
void init_colors();

/** @brief Функция начала игры или выхода из неё */
void start_or_exit_game(GameInfo_t *game_info, int key);

/** @brief Функция отрисовки всего интерфейса */
void draw_game(GameInfo_t *game_info);

/** @brief Функция отрисовки поля */
void draw_field(GameInfo_t *game_info);

/** @brief Функция отрисовки полей HUD
 * @param x Х координата левого верхнего угла
 * @param y Y координата левого верхнего угла
 * @param width Ширина поля
 * @param height Высота поля
 */
void draw_hud(int x, int y, int width, int height);

/** @brief Функция отрисовки текущей фигуры */
void draw_curr_figure(GameInfo_t *game_info);

/** @brief Функция отрисовки следующей фигуры */
void draw_next_figure(GameInfo_t *game_info);

/**
 * @brief Функция отрисовки блока фигуры
 * @param x_coord Х координата
 * @param y_coord Y координата
 * @param color цвет блока
 * */
void draw_figure_block(int x_coord, int y_coord, int color);

/** @brief Функция отрисовки элементов управления */
void draw_controls(GameInfo_t *game_info);

/**
 * @brief Функция генерации следующей фигуры
 */
void generate_next_figure(GameInfo_t *game_info);

/**
 * @brief Функция установки текущей фигуры и проверки возможности ее размещения
 * на поле
 * */
bool spawn_current_figure(GameInfo_t *game_info);

/**
 * @brief Функция обработки действий пользователя
 * @param key нажатая клавиша
 */
void userInput(GameInfo_t *game_info, int key);

/**
 * @brief Функция обработки действий пользователя во время игры
 * @param key нажатая клавиша
 */
void user_input_in_game(GameInfo_t *game_info, int key);

/**
 * @brief Функция определения ширины фигуры
 * @param figure фигура
 */
int get_figure_width(int figure[FIGURE_SIZE][FIGURE_SIZE]);

/**
 * @brief Функция определения высоты фигуры
 * @param figure фигура
 */
int get_figure_height(int figure[FIGURE_SIZE][FIGURE_SIZE]);

/**
 * @brief Функция перемещения фигуры по горизонтали
 * @param dx длина перемещения
 */
void move_figure_horizontal(GameInfo_t *game_info, int dx);

/**
 * @brief Функция поворота фигуры
 */
void rotate_figure(GameInfo_t *game_info);

/**
 * @brief Функция проверки и удаления заполненных линий
 */
void check_lines(GameInfo_t *game_info);

/**
 * @brief Функция перемещения фигуры вниз
 */
bool move_figure_down(GameInfo_t *game_info);

/**
 * @brief Функция моментального сброса фигуры
 */
void drop_figure(GameInfo_t *game_info);

/**
 * @brief Функция размещения фигуры на поле
 */
void place_figure(GameInfo_t *game_info);

/**
 * @brief Функция увеличения счета и уровня игры
 * @param lines_cnt Количество заполненных линий
 */
void increase_score_and_level(GameInfo_t *game_info, int lines_cnt);

/**
 * @brief Функция сохранения максимального счета в файл
 */
void save_max_score(GameInfo_t *game_info);

/**
 * @brief Функция загрузки максимального счета из файла
 */
void load_max_score(GameInfo_t *game_info);

/**
 * @brief Функция обновления задержки между движениями фигуры в
 * зависимости от уровня игры
 * @param timer Таймер
 */
void change_speed(GameInfo_t *game_info, int *timer);

#endif