#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

// --- Тестовые данные ---
static int game_map[ROWS_MAP][COLS_MAP];
static brick_t brick;

// --- Вспомогательная функция для проверки карты ---
// Проверяет, что в map по координатам (y, x) стоит expected_value
void assert_map_cell(int map[ROWS_MAP][COLS_MAP], int y, int x,
                     int expected_value) {
  ck_assert_int_ge(y, 0);
  ck_assert_int_ge(x, 0);
  ck_assert_int_lt(y, ROWS_MAP);
  ck_assert_int_lt(x, COLS_MAP);
  ck_assert_int_eq(map[y][x], expected_value);
}

// --- Setup ---
void setup(void) {
  // 1. Обнуляем карту (игра пуста)
  memset(game_map, 0, sizeof(game_map));
  // 2. Настраиваем кирпич (пример: форма I)
  memset(&brick, 0, sizeof(brick));
  brick.x = 3;
  brick.y = 0;
  brick.matr[0][1] = 1;
  brick.matr[1][1] = 1;
  brick.matr[2][1] = 1;
  brick.matr[3][1] = 1;
}

// --- Тесты ---
// Тест 1: Базовое "запекание" кирпича в пустую карту
START_TEST(test_bake_brick_basic) {
  // Act
  bake_brick(game_map, &brick);
  // Assert:
  assert_map_cell(game_map, 0, 4, 1);
  // brick.y + 0, brick.x + 3
  assert_map_cell(game_map, 1, 4, 1);
  assert_map_cell(game_map, 2, 4, 1);
  assert_map_cell(game_map, 3, 4, 1);
  // Assert:
  assert_map_cell(game_map, 0, 2, 0);
  assert_map_cell(game_map, 0, 5, 0);
  assert_map_cell(game_map, 1, 3, 0);
}
END_TEST

// Тест 2: Кирпич перекрывает существующие блоки на карте
START_TEST(test_bake_brick_overlap) {
  // Arrange:
  game_map[1][3] = 1;
  game_map[2][3] = 1;
  // Act
  bake_brick(game_map, &brick);
  // Assert:
  assert_map_cell(game_map, 0, 4, 1);
  assert_map_cell(game_map, 1, 3, 1);
  assert_map_cell(game_map, 2, 3, 1);
  assert_map_cell(game_map, 3, 4, 1);
}
END_TEST

// Тест 3: Кирпич с "дырками" (фигура O или T)
START_TEST(test_bake_brick_with_holes) {
  // Arrange:
  memset(&brick, 0, sizeof(brick));
  brick.x = 5;
  brick.y = 5;
  brick.matr[0][0] = 1;
  brick.matr[0][1] = 1;
  brick.matr[1][0] = 1;
  brick.matr[1][1] = 1;
  // Act
  bake_brick(game_map, &brick);
  // Assert:
  assert_map_cell(game_map, 5, 5, 1);
  assert_map_cell(game_map, 5, 6, 1);
  assert_map_cell(game_map, 6, 5, 1);
  assert_map_cell(game_map, 6, 6, 1);
}
END_TEST

// Тест 4: Кирпич у края карты (проверка границ)
START_TEST(test_bake_brick_near_edge) {
  // Arrange:
  brick.x = 8;
  brick.y = ROWS_MAP - 4;
  // Act & Assert:
  bake_brick(game_map, &brick);
  assert_map_cell(game_map, ROWS_MAP - 4, 9, 1);
  assert_map_cell(game_map, ROWS_MAP - 3, 9, 1);
}
END_TEST

// --- Suite ---
Suite *test_bake_brick_suite(void) {
  Suite *s = suite_create("suite_bake_brick");
  TCase *tc = tcase_create("Core");

  tcase_add_checked_fixture(tc, setup, NULL);
  tcase_add_test(tc, test_bake_brick_basic);
  tcase_add_test(tc, test_bake_brick_overlap);
  tcase_add_test(tc, test_bake_brick_with_holes);
  tcase_add_test(tc, test_bake_brick_near_edge);
  // Раскомментируйте для проверки на краш
  suite_add_tcase(s, tc);
  return s;
}