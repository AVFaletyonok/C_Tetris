#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

static GameInfo_t game_info;
static brick_t brick;

void check_intersection_setup(void) {
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

void check_intersection_teardown(void) {
  // Ничего чистить не нужно
}

START_TEST(test_check_intersection_floor) {
  // Помещаем кирпич на дно
  brick.y = ROWS_MAP - 1;
  bool hit = check_intersection(game_info.map, &brick);
  ck_assert_int_eq(hit, TRUE);
}
END_TEST

START_TEST(test_check_intersection_out_of_bounds_left) {
  brick.x = -2;
  bool hit = check_intersection(game_info.map, &brick);
  ck_assert_int_eq(hit, TRUE);
}
END_TEST

START_TEST(test_check_intersection_stack) {
  // Строим "пол" из блоков
  for (int j = 0; j < COLS_MAP; j++) {
    game_info.map[ROWS_MAP - 1][j] = 1;
  }
  brick.y = ROWS_MAP - 5;
  // Висим над полом
  bool hit = check_intersection(game_info.map, &brick);
  ck_assert_int_eq(hit, FALSE);
  // Пока не касаемся
  brick.y++;
  // Падаем
  hit = check_intersection(game_info.map, &brick);
  ck_assert_int_eq(hit, TRUE);  // Столкновение
}
END_TEST

Suite* test_check_intersection(void) {
  Suite* s = suite_create("suite_check_intersection");
  TCase* tc = tcase_create("Core");

  tcase_add_checked_fixture(tc, check_intersection_setup,
                            check_intersection_teardown);

  tcase_add_test(tc, test_check_intersection_floor);
  tcase_add_test(tc, test_check_intersection_out_of_bounds_left);
  tcase_add_test(tc, test_check_intersection_stack);

  suite_add_tcase(s, tc);
  return s;
}