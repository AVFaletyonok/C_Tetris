#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

static UserAction_t action;
static tetris_state state;
static GameInfo_t game_info;

void moving_setup(void) {
  state = MOVING;
  memset(&game_info, 0, sizeof(game_info));
  game_info.dir_path = ".";

  game_info_init(&game_info);
  game_info.brick.x = 3;
  game_info.brick.y = 0;
  for (int i = 0; i < BRICK_SIZE; i++) {
    game_info.brick.matr[i][1] = 1;
  }
}

START_TEST(test_moving_state_drop_down) {
  // Arrange:
  action = DROP_DOWN;
  // Act
  on_moving_state(&action, &state, &game_info);
  // Assert:
  ck_assert_int_eq(state, ATTACHING);
  ck_assert_int_eq(game_info.brick.y, ROWS_MAP - 4);
  ck_assert_int_eq(action, NOSIG);
}
END_TEST

START_TEST(test_moving_state_move_down) {
  // Arrange:
  action = MOVE_DOWN;
  // Act
  on_moving_state(&action, &state, &game_info);
  // Assert:
  ck_assert_int_eq(state, MOVING);
  ck_assert_int_eq(game_info.brick.y, 1);
  ck_assert_int_eq(action, NOSIG);
}
END_TEST

START_TEST(test_moving_state_move_right) {
  // Arrange:
  action = MOVE_RIGHT;
  // Act
  on_moving_state(&action, &state, &game_info);
  // Assert:
  ck_assert_int_eq(state, MOVING);
  ck_assert_int_eq(game_info.brick.x, 4);
  ck_assert_int_eq(action, NOSIG);
}
END_TEST

START_TEST(test_moving_state_move_left) {
  // Arrange:
  action = MOVE_LEFT;
  // Act
  on_moving_state(&action, &state, &game_info);
  // Assert:
  ck_assert_int_eq(state, MOVING);
  ck_assert_int_eq(game_info.brick.x, 2);
  ck_assert_int_eq(action, NOSIG);
}
END_TEST

START_TEST(test_moving_state_enter) {
  // Arrange:
  action = ENTER_BTN;
  // Act
  on_moving_state(&action, &state, &game_info);
  // Assert:
  ck_assert_int_eq(state, SHIFTING);
  ck_assert_int_eq(action, NOSIG);
}
END_TEST

START_TEST(test_moving_state_esc) {
  // Arrange:
  action = ESCAPE_BTN;
  // Act
  on_moving_state(&action, &state, &game_info);
  // Assert:
  ck_assert_int_eq(state, PAUSE);
  ck_assert_int_eq(action, NOSIG);
}
END_TEST

START_TEST(test_moving_state_default) {
  // Arrange:
  action = MOVE_UP;
  // Act
  on_moving_state(&action, &state, &game_info);
  // Assert:
  ck_assert_int_eq(state, MOVING);
  ck_assert_int_eq(action, NOSIG);
}
END_TEST

Suite *test_moving_state_suite(void) {
  Suite *s = suite_create("suite_moving_state");
  TCase *tc = tcase_create("Core");

  tcase_add_checked_fixture(tc, moving_setup, NULL);

  tcase_add_test(tc, test_moving_state_drop_down);
  tcase_add_test(tc, test_moving_state_move_down);
  tcase_add_test(tc, test_moving_state_move_right);
  tcase_add_test(tc, test_moving_state_move_left);
  tcase_add_test(tc, test_moving_state_enter);
  tcase_add_test(tc, test_moving_state_esc);
  tcase_add_test(tc, test_moving_state_default);

  suite_add_tcase(s, tc);
  return s;
}