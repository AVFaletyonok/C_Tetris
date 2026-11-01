#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

static tetris_state state;
static GameInfo_t game_info;

void spawn_setup(void) {
  state = MOVING;
  memset(&game_info, 0, sizeof(game_info));
  game_info.dir_path = ".";

  game_info_init(&game_info);
  game_info.next_brick.x = 3;
  game_info.next_brick.y = 0;
  for (int i = 0; i < BRICK_SIZE; i++) {
    game_info.next_brick.matr[i][1] = 1;
  }
}

START_TEST(test_spawn_state_intersect) {
  // Arrange:
  for (int j = 0; j < COLS_MAP; j++) {
    game_info.map[1][j] = 1;
  }
  // Act
  on_spawn_state(&state, &game_info);
  // Assert:
  ck_assert_int_eq(state, GAMEOVER);
}
END_TEST

START_TEST(test_spawn_state_correct) {
  // Arrange:
  // Act
  on_spawn_state(&state, &game_info);
  // Assert:
  ck_assert_int_eq(state, MOVING);
  ck_assert_int_eq(game_info.brick.y, 0);
  ck_assert_int_eq(game_info.brick.x, 3);
  ck_assert_int_eq(game_info.brick.matr[0][1], 1);
  ck_assert_int_eq(game_info.brick.matr[3][1], 1);
}
END_TEST

// --- Suite ---
Suite *test_spawn_state_suite(void) {
  Suite *s = suite_create("suite_spawn_state");
  TCase *tc = tcase_create("Core");

  tcase_add_checked_fixture(tc, spawn_setup, NULL);

  tcase_add_test(tc, test_spawn_state_intersect);
  tcase_add_test(tc, test_spawn_state_correct);

  suite_add_tcase(s, tc);
  return s;
}