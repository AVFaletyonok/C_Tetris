#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

static UserAction_t action;
static tetris_state state;
static GameInfo_t game_info;

void start_setup(void) {
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

START_TEST(test_start_state_enter) {
  // Arrange:
  action = ENTER_BTN;
  // Act
  on_start_state(&action, &state, &game_info);
  // Assert:
  ck_assert_int_eq(state, SPAWN);
}
END_TEST

START_TEST(test_start_state_esc) {
  // Arrange:
  action = ESCAPE_BTN;
  // Act
  on_start_state(&action, &state, &game_info);
  // Assert:
  ck_assert_int_eq(state, EXIT_STATE);
}
END_TEST

START_TEST(test_start_state_default) {
  // Arrange:
  action = MOVE_DOWN;
  // Act
  on_start_state(&action, &state, &game_info);
  // Assert:
  ck_assert_int_eq(state, START);
}
END_TEST

Suite *test_start_state_suite(void) {
  Suite *s = suite_create("suite_start_state");
  TCase *tc = tcase_create("Core");

  tcase_add_checked_fixture(tc, start_setup, NULL);

  tcase_add_test(tc, test_start_state_enter);
  tcase_add_test(tc, test_start_state_esc);
  tcase_add_test(tc, test_start_state_default);

  suite_add_tcase(s, tc);
  return s;
}