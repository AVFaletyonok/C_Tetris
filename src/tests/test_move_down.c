#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

static GameInfo_t game_info;
static brick_t brick;

void move_down_setup(void) {
  memset(&game_info, 0, sizeof(GameInfo_t));
  memset(&brick, 0, sizeof(brick_t));
  // game_info_init(&game_info); // Инициализируем игру

  // Настраиваем кирпич I
  brick.x = 3;
  brick.y = 0;
  for (int i = 0; i < BRICK_SIZE; i++) {
    brick.matr[i][1] = 1;
  }
}

void move_down_teardown(void) {
  // Ничего чистить не нужно
}

START_TEST(test_move_down_success) {
  tetris_state state = MOVING;
  int prev_y = brick.y;
  move_down(&state, game_info.map, &brick);
  ck_assert_int_eq(brick.y, prev_y + 1);
  ck_assert_int_eq(state, MOVING);  // Состояние не сменилось
}
END_TEST

START_TEST(test_move_down_attach) {
  tetris_state state = MOVING;
  brick.y = ROWS_MAP - 4;  // Над полом
  move_down(&state, game_info.map, &brick);
  ck_assert_int_eq(brick.y, ROWS_MAP - 4);  // Не упал
  ck_assert_int_eq(state, ATTACHING);  // Состояние сменилось на прикрепление
}
END_TEST

Suite* test_move_down(void) {
  Suite* s = suite_create("suite_move_down");
  TCase* tc = tcase_create("Core");

  tcase_add_checked_fixture(tc, move_down_setup, move_down_teardown);

  tcase_add_test(tc, test_move_down_success);
  tcase_add_test(tc, test_move_down_attach);

  suite_add_tcase(s, tc);
  return s;
}