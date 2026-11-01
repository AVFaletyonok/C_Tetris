#include "../brick_game/tetris/tetris.h"

void print_overlay() {}
void print_map(const int map[ROWS_MAP][COLS_MAP]) {}
void print_brick(const brick_t *brick) {}
void print_next_brick(const brick_t *brick) {}
void clear_backposition(const brick_t *brick) {}
void print_panel(char *msg, int len) {}
void print_stats(GameInfo_t *game_info) {}
void refresh_map(GameInfo_t *game_info) {}
void update_time(UserAction_t *action, GameInfo_t *game_info) {}
long long get_time_msec() { return 0; }
void clear_map() {}
void print_win_gameover_panel() {}
void print_lose_gameover_panel(const char *dir_path) {}