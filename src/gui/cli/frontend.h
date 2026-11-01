#ifndef FRONTEND_H
#define FRONTEND_H

#define _POSIX_C_SOURCE 200809L
#include <locale.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>

#include "../../brick_game/tetris/tetris.h"

#define WIN_INIT(time)    \
  {                       \
    initscr();            \
    noecho();             \
    curs_set(0);          \
    keypad(stdscr, TRUE); \
    timeout(time);        \
  }

#define GET_USER_INPUT getch()

#define YOU_WON_MESSAGE "Congratulations! You broke the record!"
#define YOU_WON_MESSAGE_LEN 38

#define INTRO_MESSAGE "Press Esc to exit, ENTER to start!"
#define INTRO_MESSAGE_LEN 34
#define PAUSE_MESSAGE "Press Esc to exit, ENTER to resume!"
#define PAUSE_MESSAGE_LEN 35

#define ROWS_MAP 20
#define COLS_MAP 10
#define BOARDS_BEGIN 1
#define HUD_WIDTH 20
#define WIN_WIDTH COLS_MAP + INSTRUCTION_LEN + 6

#define SUCCESS 0
#define ERROR 1

#define NO_INPUT -1

#define A_KEY 97
#define D_KEY 100
#define S_KEY 115
#define W_KEY 119

#define ESCAPE 27
#define ENTER_KEY 10

void update_time(UserAction_t* action, GameInfo_t* game_info);
long long get_time_msec(void);

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void print_overlay(void);
void print_panel(char* message, int message_len);
void print_win_gameover_panel(void);
void print_lose_gameover_panel(const char* dir_path);
void print_consolation(char* buffer);
void print_instructions(void);

void refresh_map(GameInfo_t* game_info);
void print_stats(GameInfo_t* game_info);
void clear_map(void);
void clear_backposition(const brick_t* brick);
void print_brick(const brick_t* brick);
void print_next_brick(const brick_t* brick);
void print_map(const int map[ROWS_MAP][COLS_MAP]);

#endif