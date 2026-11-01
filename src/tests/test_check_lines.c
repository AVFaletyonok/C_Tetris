#include "tetris_tests.h"

static GameInfo_t game_info;
static brick_t brick;

// void setup(void) {
//     memset(&game_info, 0, sizeof(GameInfo_t));
//     memset(&brick, 0, sizeof(brick_t));
//     // game_info_init(&game_info); // Инициализируем игру

//     // Настраиваем кирпич I
//     brick.x = 3;
//     brick.y = 0;
//     for (int i = 0; i < BRICK_SIZE; i++)
//     {
//         brick.matr[i][1] = 1;
//     }
// }

START_TEST(test_check_lines_single) {
  // Заполняем одну строку полностью
  for (int j = 0; j < COLS_MAP; j++) {
    game_info.map[ROWS_MAP - 2][j] = 1;
  }
  int count = check_lines(&game_info);
  ck_assert_int_eq(count, 1);
  // Проверяем, что строка теперь пустая (все упало вниз)
  for (int j = 0; j < COLS_MAP; j++) {
    ck_assert_int_eq(game_info.map[ROWS_MAP - 2][j], 0);
  }
}
END_TEST

START_TEST(test_check_lines_none) {
  game_info.map[0][0] = 1;
  // Только один блок
  int count = check_lines(&game_info);
  ck_assert_int_eq(count, 0);
}
END_TEST

Suite* test_check_lines(void) {
  Suite* s = suite_create("suite_check_lines");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_check_lines_single);
  tcase_add_test(tc, test_check_lines_none);

  suite_add_tcase(s, tc);
  return s;
}