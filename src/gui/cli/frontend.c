#include "frontend.h"

#include <libgen.h>
#include <limits.h>
#include <stdlib.h>

void game_loop(GameInfo_t* game_info);

int main(int argc, const char* argv[]) {
  char path_copy[PATH_MAX];
  strncpy(path_copy, argv[0], PATH_MAX - 1);
  path_copy[PATH_MAX - 1] = '\0';

  GameInfo_t game_info;
  game_info.dir_path = dirname(path_copy);

  srand(time(NULL));
  setlocale(LC_CTYPE, "");
  WIN_INIT(50);
  print_overlay();
  game_loop(&game_info);
  endwin();
  return SUCCESS;
}

void game_loop(GameInfo_t* game_info) {
  bool break_flag = TRUE;
  int signal = 0;
  UserAction_t action;
  tetris_state state = START;
  print_panel(INTRO_MESSAGE, INTRO_MESSAGE_LEN);

  while (break_flag) {
    if (state == EXIT_STATE) break_flag = FALSE;

    action = userInput(signal);
    updateCurrentState(&action, &state, game_info);

    if (state == MOVING || state == PAUSE || state == START)
      signal = GET_USER_INPUT;

    action = NOSIG;
    update_time(&action, game_info);
    if (action == MOVE_DOWN) {
      updateCurrentState(&action, &state, game_info);
    }
  }
}

void update_time(UserAction_t* action, GameInfo_t* game_info) {
  long long current_time = get_time_msec();
  long long delta_time = current_time - game_info->last_time;
  if (game_info->accumulated_time + delta_time > game_info->speed) {
    game_info->accumulated_time =
        game_info->accumulated_time + delta_time - game_info->speed;
    game_info->last_time = get_time_msec();
    *action = MOVE_DOWN;
  }
}

long long get_time_msec(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  return (long long)ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;
}

void refresh_map(GameInfo_t* game_info) {
  clear_map();
  print_overlay();
  print_map(game_info->map);
  print_brick(&game_info->brick);
  print_next_brick(&game_info->next_brick);
  print_stats(game_info);
  refresh();
}

void print_overlay(void) {
  int max_hud_len = MAX_HUD_LEN;

  print_rectangle(0, ROWS_MAP + 1, 0, COLS_MAP + 1);
  print_rectangle(0, ROWS_MAP + 1, COLS_MAP + 2, COLS_MAP + max_hud_len + 6);

  print_rectangle(1, 3, COLS_MAP + 3, COLS_MAP + max_hud_len + 5);
  print_rectangle(4, 6, COLS_MAP + 3, COLS_MAP + max_hud_len + 5);
  print_rectangle(7, 9, COLS_MAP + 3, COLS_MAP + max_hud_len + 5);
  print_rectangle(10, 15, COLS_MAP + 3, COLS_MAP + max_hud_len + 5);

  mvprintw(2, COLS_MAP + 5, "MAX SCORE");
  mvprintw(5, COLS_MAP + 5, "SCORE");
  mvprintw(8, COLS_MAP + 5, "LEVEL");
  mvprintw(13, COLS_MAP + 5, "NEXT BRICK");

  PRINT_INSTRUCTIONS();
}

void print_panel(char* message, int message_len) {
  int max_hud_len = MAX_HUD_LEN;
  print_rectangle(ROWS_MAP / 2 - 1, ROWS_MAP / 2 + 1,
                  (COLS_MAP + max_hud_len + 6 - message_len) / 2,
                  (COLS_MAP + max_hud_len + 6 + message_len) / 2 + 1);
  mvprintw(ROWS_MAP / 2, (COLS_MAP + max_hud_len + 6 - message_len) / 2 + 1,
           "%s", message);
}

void print_win_gameover_panel(void) {
  print_rectangle(ROWS_MAP / 2 - 1, ROWS_MAP / 2 + 2,
                  (WIN_WIDTH - YOU_WON_MESSAGE_LEN) / 2 - 1,
                  (WIN_WIDTH + YOU_WON_MESSAGE_LEN) / 2 + 1);

  char black_string[YOU_WON_MESSAGE_LEN + 2];
  black_string[YOU_WON_MESSAGE_LEN + 1] = '\0';
  for (int i = 0; i <= YOU_WON_MESSAGE_LEN; i++) black_string[i] = ' ';

  mvprintw(ROWS_MAP / 2, (WIN_WIDTH - YOU_WON_MESSAGE_LEN) / 2, "%s",
           black_string);
  mvprintw(ROWS_MAP / 2, (WIN_WIDTH - YOU_WON_MESSAGE_LEN) / 2, "%s",
           YOU_WON_MESSAGE);
  mvprintw(ROWS_MAP / 2 + 1, (WIN_WIDTH - YOU_WON_MESSAGE_LEN) / 2, "%s",
           black_string);
  mvprintw(ROWS_MAP / 2 + 1, (WIN_WIDTH - INTRO_MESSAGE_LEN) / 2, "%s",
           INTRO_MESSAGE);
}

void print_lose_gameover_panel(const char* dir_path) {
  char file_path[PATH_MAX];
  snprintf(file_path, PATH_MAX, "%s/%s", dir_path, YOU_LOSE_FILE);

  char buffer[256];
  print_rectangle(ROWS_MAP / 2 - 2, ROWS_MAP / 2 + 2,
                  (WIN_WIDTH - YOU_WON_MESSAGE_LEN) / 2 - 1,
                  (WIN_WIDTH + YOU_WON_MESSAGE_LEN) / 2 + 1);
  buffer[YOU_WON_MESSAGE_LEN + 1] = '\0';
  for (int i = 0; i < YOU_WON_MESSAGE_LEN + 1; i++) buffer[i] = ' ';

  mvprintw(ROWS_MAP / 2 - 1, (WIN_WIDTH - YOU_WON_MESSAGE_LEN) / 2, "%s",
           buffer);
  mvprintw(ROWS_MAP / 2, (WIN_WIDTH - YOU_WON_MESSAGE_LEN) / 2, "%s", buffer);
  mvprintw(ROWS_MAP / 2 + 1, (WIN_WIDTH - YOU_WON_MESSAGE_LEN) / 2, "%s",
           buffer);
  mvprintw(ROWS_MAP / 2 + 1, (WIN_WIDTH - INTRO_MESSAGE_LEN) / 2, "%s",
           INTRO_MESSAGE);

  FILE* file = fopen(file_path, "r");
  if (file != NULL) {
    int line_count = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
      line_count++;
    }
    rewind(file);

    if (line_count > 0) {
      int rand_line_index = 0;
      rand_line_index = rand() % line_count;
      for (int i = 0; i <= rand_line_index; i++) {
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\n")] = '\0';
      }
      print_consolation(buffer);
    }
    fclose(file);
  }
  refresh();
}

void print_consolation(char* buffer) {
  int len = strlen(buffer);
  int printed_len = 0;
  while (printed_len < len) {
    int cur_len = YOU_WON_MESSAGE_LEN;
    if (len - printed_len < cur_len) {
      cur_len = len - printed_len;
    }
    while (buffer[printed_len + cur_len] != ',' &&
           buffer[printed_len + cur_len] != '.' &&
           buffer[printed_len + cur_len] != '!' &&
           buffer[printed_len + cur_len] != '?') {
      cur_len--;
    }
    cur_len++;
    buffer[printed_len + cur_len] = '\0';

    int y = (printed_len + cur_len) / YOU_WON_MESSAGE_LEN;
    mvprintw(ROWS_MAP / 2 - 1 + y, (WIN_WIDTH - cur_len) / 2 + 1, "%s",
             buffer + printed_len);
    printed_len = printed_len + cur_len + 1;
  }
}

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  mvaddch(top_y, left_x, ACS_ULCORNER);

  int i;
  for (i = left_x + 1; i < right_x; i++) mvaddch(top_y, i, ACS_HLINE);
  mvaddch(top_y, i, ACS_URCORNER);

  for (i = top_y + 1; i < bottom_y; i++) {
    mvaddch(i, left_x, ACS_VLINE);
    mvaddch(i, right_x, ACS_VLINE);
  }

  mvaddch(bottom_y, left_x, ACS_LLCORNER);
  for (i = left_x + 1; i < right_x; i++) mvaddch(bottom_y, i, ACS_HLINE);
  mvaddch(bottom_y, i, ACS_LRCORNER);
}

void print_stats(GameInfo_t* game_info) {
  int max_hud_len = MAX_HUD_LEN;
  mvprintw(2, COLS_MAP + max_hud_len + 1, "%4d", game_info->high_score);
  mvprintw(5, COLS_MAP + max_hud_len + 1, "%4d", game_info->score);
  mvprintw(8, COLS_MAP + max_hud_len + 1, "%4d", game_info->level);
}

void clear_map(void) {
  int len = WIN_WIDTH;
  char clean_str[len + 1];

  for (int j = 0; j < len; j++) {
    clean_str[j] = ' ';
  }
  clean_str[len] = '\0';

  for (int i = 0; i < ROWS_MAP; i++)
    mvprintw(BOARDS_BEGIN + i, BOARDS_BEGIN, "%s", clean_str);
}

void clear_backposition(const brick_t* brick) {
  for (int i = 0; i < BRICK_SIZE; i++) {
    for (int j = 0; j < BRICK_SIZE; j++) {
      if (brick->matr[i][j] == 1) {
        mvaddch(BOARDS_BEGIN + brick->y + i, BOARDS_BEGIN + brick->x + j, ' ');
      }
    }
  }
}

void print_brick(const brick_t* brick) {
  for (int i = 0; i < BRICK_SIZE; i++) {
    for (int j = 0; j < BRICK_SIZE; j++) {
      if (brick->matr[i][j] == 1) {
        mvaddch(BOARDS_BEGIN + brick->y + i, BOARDS_BEGIN + brick->x + j,
                BRICK_SIMBOL);
      }
    }
  }
}

void print_next_brick(const brick_t* brick) {
  int max_hud_len = MAX_HUD_LEN;
  for (int i = 0; i < BRICK_SIZE; i++) {
    for (int j = 0; j < BRICK_SIZE; j++) {
      if (brick->matr[i][j] == 1) {
        mvaddch(11 + i, COLS_MAP + max_hud_len + j, BRICK_SIMBOL);
      } else {
        mvaddch(11 + i, COLS_MAP + max_hud_len + j, ' ');
      }
    }
  }
}

void print_map(const int map[ROWS_MAP][COLS_MAP]) {
  for (int i = 0; i < ROWS_MAP; i++) {
    for (int j = 0; j < COLS_MAP; j++) {
      if (map[i][j] == 1) {
        mvaddch(BOARDS_BEGIN + i, BOARDS_BEGIN + j, BRICK_SIMBOL);
      } else {
        mvaddch(BOARDS_BEGIN + i, BOARDS_BEGIN + j, ' ');
      }
    }
  }
  refresh();
}