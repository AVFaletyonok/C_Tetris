#include "tetris.h"

void updateCurrentState(UserAction_t* action, tetris_state* state,
                        GameInfo_t* game_info) {
  switch (*state) {
    case START:
      on_start_state(action, state, game_info);
      break;
    case PAUSE:
      on_pause_state(action, state, game_info);
      break;
    case SPAWN:
      on_spawn_state(state, game_info);
      break;
    case MOVING:
      on_moving_state(action, state, game_info);
      break;
    case SHIFTING:
      on_shifting_state(state, game_info);
      break;
    case ATTACHING:
      on_attaching_state(state, game_info);
      break;
    case GAMEOVER:
      on_gameover_state(state, game_info);
      break;
    default:
      break;
  }
}

UserAction_t userInput(int user_input) {
  UserAction_t action = NOSIG;

  switch (user_input) {
    case KEY_UP:
    case W_KEY:
      action = MOVE_UP;
      break;
    case KEY_DOWN:
    case S_KEY:
      action = DROP_DOWN;
      break;
    case KEY_LEFT:
    case A_KEY:
      action = MOVE_LEFT;
      break;
    case KEY_RIGHT:
    case D_KEY:
      action = MOVE_RIGHT;
      break;
    case ESCAPE:
      action = ESCAPE_BTN;
      break;
    case ENTER_KEY:
      action = ENTER_BTN;
      break;
    default:
      break;
  }
  return action;
}

void on_start_state(UserAction_t* action, tetris_state* state,
                    GameInfo_t* game_info) {
  switch (*action) {
    case ENTER_BTN:
      game_info_init(game_info);
      clear_map();
      print_overlay();
      print_map(game_info->map);
      print_stats(game_info);
      *state = SPAWN;
      break;
    case ESCAPE_BTN:
      *state = EXIT_STATE;
      break;
    default:
      *state = START;
      break;
  }
  *action = NOSIG;
}

void on_pause_state(UserAction_t* action, tetris_state* state,
                    GameInfo_t* game_info) {
  update_time(action, game_info);
  print_panel(PAUSE_MESSAGE, PAUSE_MESSAGE_LEN);

  switch (*action) {
    case ENTER_BTN:
      refresh_map(game_info);
      game_info->last_time = get_time_msec();
      *state = MOVING;
      break;
    case ESCAPE_BTN:
      *state = EXIT_STATE;
      break;
    default:
      *state = PAUSE;
      break;
  }
  *action = NOSIG;
}

void on_spawn_state(tetris_state* state, GameInfo_t* game_info) {
  game_info->brick = game_info->next_brick;
  create_rnd_brick(&game_info->next_brick, game_info->brick_start_set);

  print_brick(&game_info->brick);
  print_next_brick(&game_info->next_brick);

  bool is_intersect = check_intersection(game_info->map, &game_info->brick);
  if (is_intersect) {
    *state = GAMEOVER;
  } else {
    *state = MOVING;
  }
}

void on_moving_state(UserAction_t* action, tetris_state* state,
                     GameInfo_t* game_info) {
  switch (*action) {
    case DROP_DOWN:
      drop_down(state, game_info->map, &game_info->brick);
      break;
    case MOVE_DOWN:
      move_down(state, game_info->map, &game_info->brick);
      break;
    case MOVE_RIGHT:
      move_right(game_info->map, &game_info->brick);
      break;
    case MOVE_LEFT:
      move_left(game_info->map, &game_info->brick);
      break;
    case ENTER_BTN:
      *state = SHIFTING;
      break;
    case ESCAPE_BTN:
      *state = PAUSE;
      break;
    default:
      break;
  }
  *action = NOSIG;
}

void on_shifting_state(tetris_state* state, GameInfo_t* game_info) {
  brick_t temp_brick;
  temp_brick.x = game_info->brick.x;
  temp_brick.y = game_info->brick.y;
  for (int i = 0; i < BRICK_SIZE; i++)
    for (int j = 0; j < BRICK_SIZE; j++)
      temp_brick.matr[j][BRICK_SIZE - 1 - i] = game_info->brick.matr[i][j];
  bool is_intersect = check_intersection(game_info->map, &temp_brick);
  if (!is_intersect) {
    clear_backposition(&game_info->brick);
    for (int i = 0; i < BRICK_SIZE; i++) {
      memcpy(game_info->brick.matr[i], temp_brick.matr[i],
             BRICK_SIZE * sizeof(int));
    }
    print_brick(&game_info->brick);
  }
  *state = MOVING;
}

void on_attaching_state(tetris_state* state, GameInfo_t* game_info) {
  bake_brick(game_info->map, &game_info->brick);
  int deleted_lines_count = check_lines(game_info);
  if (deleted_lines_count >= BONUSES_COUNT)
    deleted_lines_count = BONUSES_COUNT - 1;

  if (deleted_lines_count > 0) {
    const int bonuses[BONUSES_COUNT] = BONUSES;
    game_info->score += bonuses[deleted_lines_count];
    if (game_info->score > game_info->high_score) {
      game_info->high_score = game_info->score;
      game_info->broke_record = true;
    }
    game_info->level = game_info->score / SCORE_TO_SPEED_UP + 1;
    if (game_info->level <= 10) {
      change_speed(game_info);
    } else {
      game_info->level = 10;
    }
  }
  print_map(game_info->map);
  print_stats(game_info);
  *state = SPAWN;
}

void on_gameover_state(tetris_state* state, const GameInfo_t* game_info) {
  if (game_info->broke_record) {
    save_high_score(game_info->high_score, game_info->dir_path);
    print_win_gameover_panel();
  } else {
    print_lose_gameover_panel(game_info->dir_path);
  }
  *state = START;
}

bool check_intersection(const int map[ROWS_MAP][COLS_MAP],
                        const brick_t* brick) {
  bool result = FALSE;
  for (int i = 0; i < BRICK_SIZE && !result; i++) {
    for (int j = 0; j < BRICK_SIZE && !result; j++) {
      if (brick->matr[i][j] == 1 &&
          (brick->x + j < 0 || brick->x + j >= COLS_MAP ||
           brick->y + i >= ROWS_MAP || map[brick->y + i][brick->x + j] == 1)) {
        result = TRUE;
      }
    }
  }
  return result;
}

void bake_brick(int map[ROWS_MAP][COLS_MAP], const brick_t* brick) {
  for (int i = 0; i < BRICK_SIZE; i++) {
    for (int j = 0; j < BRICK_SIZE; j++) {
      if (brick->matr[i][j] == 1) {
        map[brick->y + i][brick->x + j] = 1;
      }
    }
  }
}

void delete_line(int map[ROWS_MAP][COLS_MAP], int index) {
  for (int i = index; i > 0; i--) {
    bool flag_empty_line = TRUE;
    for (int j = 0; j < COLS_MAP; j++) {
      map[i][j] = map[i - 1][j];
      if (map[i - 1][j] == 1) flag_empty_line = FALSE;
    }
    if (flag_empty_line) break;
  }
  for (int j = 0; j < COLS_MAP; j++) map[0][j] = 0;
}

int check_lines(GameInfo_t* game_info) {
  int count = 0;
  for (int i = ROWS_MAP - 1; i >= 0; i--) {
    bool flag_full_line = TRUE;
    for (int j = 0; j < COLS_MAP; j++) {
      if (game_info->map[i][j] == 0) {
        flag_full_line = FALSE;
        break;
      }
    }
    if (flag_full_line) {
      delete_line(game_info->map, i);
      count++;
      i++;
    }
  }
  return count;
}

void change_speed(GameInfo_t* game_info) {
  if (game_info->score > SCORE_TO_SPEED_UP) {
    game_info->speed = START_SPEED;
    int n = game_info->score / SCORE_TO_SPEED_UP;
    if (n > MAX_LEVEL) {
      n = MAX_LEVEL;
    }
    for (int i = 0; i < n; i++) {
      game_info->speed *= SPEED_INCREASE;
    }
  }
}

void move_down(tetris_state* state, int map[ROWS_MAP][COLS_MAP],
               brick_t* brick) {
  clear_backposition(brick);
  brick->y += 1;
  bool is_intersect = check_intersection(map, brick);
  if (is_intersect) {
    brick->y -= 1;
    *state = ATTACHING;
  }
  print_brick(brick);
}

void drop_down(tetris_state* state, int map[ROWS_MAP][COLS_MAP],
               brick_t* brick) {
  int prev_y = brick->y;
  move_down(state, map, brick);
  while (prev_y != brick->y) {
    prev_y = brick->y;
    move_down(state, map, brick);
  }
}

void move_right(int map[ROWS_MAP][COLS_MAP], brick_t* brick) {
  clear_backposition(brick);
  brick->x += 1;
  bool is_intersect = check_intersection(map, brick);
  if (is_intersect) {
    brick->x -= 1;
  }
  print_brick(brick);
}

void move_left(int map[ROWS_MAP][COLS_MAP], brick_t* brick) {
  clear_backposition(brick);
  brick->x -= 1;
  bool is_intersect = check_intersection(map, brick);
  if (is_intersect) {
    brick->x += 1;
  }
  print_brick(brick);
}

void rotate_clockwise(int map[ROWS_MAP][COLS_MAP], brick_t* brick) {
  brick_t temp_brick;
  temp_brick.x = brick->x;
  temp_brick.y = brick->y;
  for (int i = 0; i < BRICK_SIZE; i++)
    for (int j = 0; j < BRICK_SIZE; j++)
      temp_brick.matr[j][BRICK_SIZE - 1 - i] = brick->matr[i][j];
  bool is_intersect = check_intersection(map, &temp_brick);
  if (!is_intersect) {
    clear_backposition(brick);
    for (int i = 0; i < BRICK_SIZE; i++) {
      memcpy(brick->matr[i], temp_brick.matr[i], BRICK_SIZE * sizeof(int));
    }
    print_brick(brick);
  }
}

int read_high_score(const char* dir_path) {
  int result = 0;

  if (dir_path != NULL) {
    char file_path[PATH_MAX];
    snprintf(file_path, PATH_MAX, "%s/%s", dir_path, HIGH_SCORE_FILE);
    FILE* file = fopen(file_path, "r");

    if (file != NULL) {
      fscanf(file, "%d", &result);
      fclose(file);
    }
  }
  return result;
}

void save_high_score(int high_score, const char* dir_path) {
  char file_path[PATH_MAX];

  snprintf(file_path, PATH_MAX, "%s/%s", dir_path, HIGH_SCORE_FILE);
  FILE* file = fopen(file_path, "w");

  if (file != NULL) {
    fprintf(file, "%d", high_score);
    fclose(file);
  }
}

void initialise_matrix(int matr[ROWS_MAP][COLS_MAP], int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      matr[i][j] = 0;
    }
  }
}

void initialise_brick_set(brick_t* brick_set) {
  for (int i = 0; i < BRICK_SET_COUNT; i++) {
    brick_set[i].x = 3;
    brick_set[i].y = 0;
  }

  for (int k = 0; k < BRICK_SET_COUNT; k++)
    for (int i = 0; i < BRICK_SIZE; i++)
      for (int j = 0; j < BRICK_SIZE; j++) brick_set[k].matr[i][j] = 0;

  for (int i = 0; i < BRICK_SIZE; i++) brick_set[0].matr[i][1] = 1;  // I

  brick_set[1].matr[0][2] = brick_set[1].matr[1][2] = brick_set[1].matr[2][1] =
      brick_set[1].matr[2][2] = 1;  // J
  brick_set[2].matr[0][1] = brick_set[2].matr[1][1] = brick_set[2].matr[2][1] =
      brick_set[2].matr[2][2] = 1;  // L
  brick_set[3].matr[1][1] = brick_set[3].matr[1][2] = brick_set[3].matr[2][1] =
      brick_set[3].matr[2][2] = 1;  // O
  brick_set[4].matr[1][1] = brick_set[4].matr[1][2] = brick_set[4].matr[2][0] =
      brick_set[4].matr[2][1] = 1;  // S
  brick_set[5].matr[2][0] = brick_set[5].matr[2][1] = brick_set[5].matr[2][2] =
      brick_set[5].matr[1][1] = 1;  // T
  brick_set[6].matr[1][0] = brick_set[6].matr[1][1] = brick_set[6].matr[2][1] =
      brick_set[6].matr[2][2] = 1;  // Z
}

void create_rnd_brick(brick_t* new_brick, const brick_t* brick_set) {
  int index = rand() % BRICK_SET_COUNT;
  *new_brick = brick_set[index];
}

void game_info_init(GameInfo_t* game_info) {
  game_info->broke_record = false;
  game_info->high_score = read_high_score(game_info->dir_path);
  game_info->score = 0;
  game_info->level = 1;
  game_info->speed = START_SPEED;
  game_info->accumulated_time = 0;
  game_info->last_time = get_time_msec();
  initialise_matrix(game_info->map, ROWS_MAP, COLS_MAP);
  initialise_brick_set(game_info->brick_start_set);  // I, J, L, O, S, T, Z
  create_rnd_brick(&game_info->next_brick, game_info->brick_start_set);
}