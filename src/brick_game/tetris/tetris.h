#ifndef TETRIS_H
#define TETRIS_H

#define _POSIX_C_SOURCE 200809L
#include <limits.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define ROWS_MAP 20
#define COLS_MAP 10

#define YOU_LOSE_FILE "./data/consolations.txt"
#define HIGH_SCORE_FILE "./data/records.txt"

#define INSTRUCTIONS_COUNT 5
#define INSTRUCTION_LEN 26
#define PRINT_INSTRUCTIONS()                                  \
  {                                                           \
    mvprintw(16, COLS_MAP + 4, "Esc - Pause/Exit");           \
    mvprintw(17, COLS_MAP + 4, "A/ARROW_LEFT - move left");   \
    mvprintw(18, COLS_MAP + 4, "S/ARROW_DOWN - move down");   \
    mvprintw(19, COLS_MAP + 4, "D/ARROW_RIGHT - move right"); \
    mvprintw(20, COLS_MAP + 4, "Enter - Rotate/Resume");      \
  }
#define MAX_HUD_LEN \
  { HUD_WIDTH > INSTRUCTION_LEN ? HUD_WIDTH : INSTRUCTION_LEN }
#define MAX_LEVEL 10

#define BRICK_SIMBOL '#'
#define BRICK_SIZE 4
#define BRICK_SET_COUNT 7

#define START_SPEED 800      // in milliseconds
#define SPEED_INCREASE 0.90  // acceleration +10% per each level
#define SCORE_TO_SPEED_UP 600

#define BONUSES_COUNT 5
#define BONUSES \
  { 0, 100, 300, 700, 1500 }

typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
  PAUSE,
  EXIT_STATE
} tetris_state;

typedef enum {
  MOVE_UP = 0,
  MOVE_DOWN,
  DROP_DOWN,
  MOVE_RIGHT,
  MOVE_LEFT,
  ESCAPE_BTN,  // = PAUSE
  ENTER_BTN,   // = ACTION
  NOSIG
} UserAction_t;

typedef struct {
  int x;
  int y;
  int matr[BRICK_SIZE][BRICK_SIZE];
} brick_t;

typedef struct {
  char* dir_path;
  int score;
  bool broke_record;
  int high_score;
  int level;
  int speed;  // in milliseconds
  long long last_time;
  long accumulated_time;
  int map[ROWS_MAP][COLS_MAP];
  brick_t brick_start_set[BRICK_SET_COUNT];
  brick_t brick;
  brick_t next_brick;
} GameInfo_t;

typedef struct {
  int max_score;
  int score;
  int level;
  int speed;
} game_stats_t;

#include "../../gui/cli/frontend.h"

void updateCurrentState(UserAction_t* sig, tetris_state* state,
                        GameInfo_t* game_info);
UserAction_t userInput(int user_input);

void on_start_state(UserAction_t* action, tetris_state* state,
                    GameInfo_t* game_info);
void on_pause_state(UserAction_t* action, tetris_state* state,
                    GameInfo_t* game_info);
void on_spawn_state(tetris_state* state, GameInfo_t* game_info);
void on_moving_state(UserAction_t* action, tetris_state* state,
                     GameInfo_t* game_info);
void on_shifting_state(tetris_state* state, GameInfo_t* game_info);
void on_attaching_state(tetris_state* state, GameInfo_t* game_info);
void on_gameover_state(tetris_state* state, const GameInfo_t* game_info);

void initialise_matrix(int matr[ROWS_MAP][COLS_MAP], int n, int m);
void initialise_brick_set(brick_t* brick_set);
void create_rnd_brick(brick_t* new_brick, const brick_t* brick_set);
int read_high_score(const char* dir_path);
void save_high_score(int high_score, const char* dir_path);
void game_info_init(GameInfo_t* game_info);

void delete_line(int map[ROWS_MAP][COLS_MAP], int index);
void change_speed(GameInfo_t* game_info);
bool check_intersection(const int map[ROWS_MAP][COLS_MAP],
                        const brick_t* brick);
int check_lines(GameInfo_t* game_info);
void move_down(tetris_state* state, int map[ROWS_MAP][COLS_MAP],
               brick_t* brick);
void bake_brick(int map[ROWS_MAP][COLS_MAP], const brick_t* brick);
void drop_down(tetris_state* state, int map[ROWS_MAP][COLS_MAP],
               brick_t* brick);
void move_right(int map[ROWS_MAP][COLS_MAP], brick_t* brick);
void move_left(int map[ROWS_MAP][COLS_MAP], brick_t* brick);
void rotate_clockwise(int map[ROWS_MAP][COLS_MAP], brick_t* brick);

#endif