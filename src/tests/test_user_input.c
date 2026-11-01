#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

// Тест: Стрелка вверх и клавиша W
START_TEST(test_user_input_move_up) {
  UserAction_t action;
  action = userInput(KEY_UP);
  ck_assert_int_eq(action, MOVE_UP);
  action = userInput(W_KEY);
  ck_assert_int_eq(action, MOVE_UP);
}
END_TEST

// Тест: Стрелка вниз и клавиша S
START_TEST(test_user_input_drop_down) {
  UserAction_t action;
  action = userInput(KEY_DOWN);
  ck_assert_int_eq(action, DROP_DOWN);
  action = userInput(S_KEY);
  ck_assert_int_eq(action, DROP_DOWN);
}
END_TEST

// Тест: Стрелка влево и клавиша A
START_TEST(test_user_input_move_left) {
  UserAction_t action;
  action = userInput(KEY_LEFT);
  ck_assert_int_eq(action, MOVE_LEFT);
  action = userInput(A_KEY);
  ck_assert_int_eq(action, MOVE_LEFT);
}
END_TEST

// Тест: Стрелка вправо и клавиша D
START_TEST(test_user_input_move_right) {
  UserAction_t action;
  action = userInput(KEY_RIGHT);
  ck_assert_int_eq(action, MOVE_RIGHT);
  action = userInput(D_KEY);
  ck_assert_int_eq(action, MOVE_RIGHT);
}
END_TEST

// Тест: Кнопки действия
START_TEST(test_user_input_actions) {
  UserAction_t action;
  action = userInput(ESCAPE);
  ck_assert_int_eq(action, ESCAPE_BTN);
  action = userInput(ENTER_KEY);
  ck_assert_int_eq(action, ENTER_BTN);
}
END_TEST

// Тест: Неизвестная клавиша (дефолтный случай)
START_TEST(test_user_input_default) {
  UserAction_t action;
  action = userInput(999);
  // Случайное число, которого нет в switch
  ck_assert_int_eq(action, NOSIG);
  action = userInput(0);
  ck_assert_int_eq(action, NOSIG);
}
END_TEST

Suite *test_user_input(void) {
  Suite *s = suite_create("suite_user_input");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_user_input_move_up);
  tcase_add_test(tc_core, test_user_input_drop_down);
  tcase_add_test(tc_core, test_user_input_move_left);
  tcase_add_test(tc_core, test_user_input_move_right);
  tcase_add_test(tc_core, test_user_input_actions);
  tcase_add_test(tc_core, test_user_input_default);

  suite_add_tcase(s, tc_core);
  return s;
}