#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

// --- Тестовые данные ---
static int game_map[ROWS_MAP][COLS_MAP];
static brick_t brick;

// --- Setup ---
void rotate_setup(void) {
  // 1. Очищаем карту
  memset(game_map, 0, sizeof(game_map));
  // 2. Настраиваем кирпич (форма I)
  memset(&brick, 0, sizeof(brick));
  brick.x = 3;
  brick.y = 0;
  for (int i = 0; i < BRICK_SIZE; i++) {
    brick.matr[i][1] = 1;
  }
}

// --- Тесты ---
// Тест 1: Успешный поворот (свободное место)
START_TEST(test_rotate_success) {
  // Act
  rotate_clockwise(game_map, &brick);
  bake_brick(game_map, &brick);
  // Assert:
  ck_assert_int_eq(brick.x, 3);
  ck_assert_int_eq(game_map[1][3], 1);
  ck_assert_int_eq(game_map[1][6], 1);
  ck_assert_int_eq(game_map[0][4], 0);
  ck_assert_int_eq(game_map[2][4], 0);
}
END_TEST

// Тест 2: Столкновение с блоком на карте (Collision)
START_TEST(test_rotate_collision_with_block) {
  // Arrange: "Запекаем" блок на карте прямо на пути кирпича
  // Кирпич стоит в x=3. Поставим блок в x=5 (на соседней клетке)
  game_map[0][5] = 1;
  game_map[1][5] = 1;
  game_map[2][5] = 1;
  game_map[3][5] = 1;
  // Act
  rotate_clockwise(game_map, &brick);
  bake_brick(game_map, &brick);
  // Assert:
  ck_assert_int_eq(brick.x, 3);
  ck_assert_int_eq(game_map[1][3], 0);
  ck_assert_int_eq(game_map[1][6], 0);
  ck_assert_int_eq(game_map[0][4], 1);
  ck_assert_int_eq(game_map[3][4], 1);
}
END_TEST

Suite *test_rotate_suite(void) {
  Suite *s = suite_create("suite_rotate_clockwise");
  TCase *tc = tcase_create("Core");

  tcase_add_checked_fixture(tc, rotate_setup, NULL);

  tcase_add_test(tc, test_rotate_success);
  tcase_add_test(tc, test_rotate_collision_with_block);

  suite_add_tcase(s, tc);
  return s;
}