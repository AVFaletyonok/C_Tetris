#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

static GameInfo_t game_info;
static tetris_state state;

void shifting_setup(void) {
  state = SHIFTING;
  memset(&game_info, 0, sizeof(game_info));
  game_info.dir_path = ".";

  game_info_init(&game_info);
  game_info.brick.x = 3;
  game_info.brick.y = 0;
  for (int i = 0; i < BRICK_SIZE; i++) {
    game_info.brick.matr[i][1] = 1;
  }
}

START_TEST(test_shifting_state_correct) {
  // Arrange:
  // Act
  on_shifting_state(&state, &game_info);
  // Assert:
  ck_assert_int_eq(state, MOVING);
  ck_assert_int_eq(game_info.brick.matr[0][1], 0);
  ck_assert_int_eq(game_info.brick.matr[1][0], 1);
  ck_assert_int_eq(game_info.brick.matr[1][3], 1);
}
END_TEST

START_TEST(test_shifting_state_wrong) {
  // Arrange:
  game_info.brick.x = COLS_MAP - 2;
  // Act
  on_shifting_state(&state, &game_info);
  // Assert:
  ck_assert_int_eq(state, MOVING);
  ck_assert_int_eq(game_info.brick.matr[0][1], 1);
  ck_assert_int_eq(game_info.brick.matr[3][1], 1);
  ck_assert_int_eq(game_info.brick.matr[1][0], 0);
}
END_TEST

// --- Suite ---
Suite *test_shifting_state_suite(void) {
  Suite *s = suite_create("suite_shifting_state");
  TCase *tc = tcase_create("Core");

  tcase_add_checked_fixture(tc, shifting_setup, NULL);

  tcase_add_test(tc, test_shifting_state_correct);
  tcase_add_test(tc, test_shifting_state_wrong);

  suite_add_tcase(s, tc);
  return s;
}