#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

// --- Тестовые данные ---
static int game_map[ROWS_MAP][COLS_MAP];
static brick_t brick;

// --- Setup ---
void move_right_setup(void) {
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
// Тест 1: Успешное движение вправо (свободное место)
START_TEST(test_move_right_success) {
  // Act
  move_right(game_map, &brick);
  // Assert: Координата изменилась
  ck_assert_int_eq(brick.x, 4);
}
END_TEST

// Тест 2: Столкновение с блоком на карте (Collision)
START_TEST(test_move_right_collision_with_block) {
  // Arrange: "Запекаем" блок на карте прямо на пути кирпича
  // Кирпич стоит в x=3. Поставим блок в x=5 (на соседней клетке)
  game_map[0][5] = 1;
  game_map[1][5] = 1;
  game_map[2][5] = 1;
  game_map[3][5] = 1;
  // Act
  move_right(game_map, &brick);
  // Assert: Координата НЕ должна измениться из-за столкновения
  ck_assert_int_eq(brick.x, 3);
}
END_TEST

// Тест 3: Столкновение с правой стеной (Boundary)
START_TEST(test_move_right_collision_with_wall) {
  // Arrange: Ставим кирпич вплотную к правой границе
  brick.x = COLS_MAP - 2;
  // Act
  move_right(game_map, &brick);
  // Assert:
  ck_assert_int_eq(brick.x, COLS_MAP - 2);
}
END_TEST

// --- Suite ---
Suite *test_move_right_suite(void) {
  Suite *s = suite_create("suite_move_right");
  TCase *tc = tcase_create("Core");

  tcase_add_checked_fixture(tc, move_right_setup, NULL);

  tcase_add_test(tc, test_move_right_success);
  tcase_add_test(tc, test_move_right_collision_with_block);
  tcase_add_test(tc, test_move_right_collision_with_wall);

  suite_add_tcase(s, tc);
  return s;
}