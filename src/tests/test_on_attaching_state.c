#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

static GameInfo_t game_info;
static tetris_state state;

void attaching_setup(void) {
  state = MOVING;
  memset(&game_info, 0, sizeof(game_info));
  game_info.dir_path = ".";

  game_info_init(&game_info);
  game_info.brick.x = 3;
  game_info.brick.y = ROWS_MAP - 4;
  for (int i = 0; i < BRICK_SIZE; i++) {
    game_info.brick.matr[i][1] = 1;
  }
  for (int j = 0; j < COLS_MAP; j++) {
    game_info.map[ROWS_MAP - 1][j] = 1;
  }
}

START_TEST(test_attaching_state_zero_high_score) {
  // Arrange:
  game_info.high_score = 0;
  // Act
  on_attaching_state(&state, &game_info);
  // Assert:
  ck_assert_int_eq(state, SPAWN);
  ck_assert_int_eq(game_info.score, 100);
  ck_assert_int_eq(game_info.level, 1);
}
END_TEST

START_TEST(test_attaching_state_level_up) {
  // Arrange:
  game_info.score = 500;
  game_info.high_score = 600;
  // Act
  on_attaching_state(&state, &game_info);
  // Assert:
  ck_assert_int_eq(state, SPAWN);
  ck_assert_int_eq(game_info.score, 600);
  ck_assert_int_eq(game_info.level, 2);
}
END_TEST

START_TEST(test_attaching_state_11_level) {
  // Arrange:
  game_info.score = 6500;
  game_info.level = 10;
  // Act
  on_attaching_state(&state, &game_info);
  // Assert:
  ck_assert_int_eq(state, SPAWN);
  ck_assert_int_eq(game_info.score, 6600);
  ck_assert_int_eq(game_info.level, 10);
}
END_TEST

// --- Suite ---
Suite *test_attaching_state_suite(void) {
  Suite *s = suite_create("suite_attaching_state");
  TCase *tc = tcase_create("Core");

  tcase_add_checked_fixture(tc, attaching_setup, NULL);

  tcase_add_test(tc, test_attaching_state_zero_high_score);
  tcase_add_test(tc, test_attaching_state_level_up);
  tcase_add_test(tc, test_attaching_state_11_level);

  suite_add_tcase(s, tc);
  return s;
}