#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

static GameInfo_t game_info;
static tetris_state state;

void gameover_setup(void) {
  state = MOVING;
  memset(&game_info, 0, sizeof(game_info));
  game_info.dir_path = ".";
}

START_TEST(test_gameover_win) {
  // Arrange:
  game_info.broke_record = true;
  // Act
  on_gameover_state(&state, &game_info);
  // Assert:
  ck_assert_int_eq(state, START);
}
END_TEST

START_TEST(test_gameover_loose) {
  // Arrange:
  game_info.broke_record = false;
  // Act
  on_gameover_state(&state, &game_info);
  // Assert:
  ck_assert_int_eq(state, START);
}
END_TEST

// --- Suite ---
Suite *test_gameover_suite(void) {
  Suite *s = suite_create("suite_gameover");
  TCase *tc = tcase_create("Core");

  tcase_add_checked_fixture(tc, gameover_setup, NULL);

  tcase_add_test(tc, test_gameover_win);
  tcase_add_test(tc, test_gameover_loose);

  suite_add_tcase(s, tc);
  return s;
}