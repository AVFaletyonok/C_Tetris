#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

#define TEST_ROWS 20
#define TEST_COLS 10
// Используем фиксированный массив для теста, чтобы не зависеть от
// ROWS_MAP/COLS_MAP
static int test_matrix[TEST_ROWS][TEST_COLS];

// Тест 1: Базовая проверка (все элементы стали 0)
START_TEST(test_initialise_matrix_basic) {
  for (int i = 0; i < TEST_ROWS; i++) {
    for (int j = 0; j < TEST_COLS; j++) {
      test_matrix[i][j] = 999;
    }
  }
  // Act
  initialise_matrix(test_matrix, TEST_ROWS, TEST_COLS);
  // Assert: проверяем КАЖДЫЙ элемент
  for (int i = 0; i < TEST_ROWS; i++) {
    for (int j = 0; j < TEST_COLS; j++) {
      ck_assert_int_eq(test_matrix[i][j], 0);
    }
  }
}
END_TEST

// Тест 2: Нулевые размеры (Edge case)
START_TEST(test_initialise_matrix_zero_size) {
  int small_matrix[20][10];
  memset(small_matrix, 1, sizeof(small_matrix));
  // Act: просим инициализировать 0 строк или 0 столбцов
  initialise_matrix(small_matrix, 20, 0);
  // Assert: данные ВНЕЗАПНО не должны измениться (циклы просто не выполнятся)
  // Проверяем один элемент для примера
  ck_assert_int_eq(small_matrix[0][0], 16843009);
}
END_TEST

Suite *test_initialise_matrix_suite(void) {
  Suite *s = suite_create("suite_initialise_matrix");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_initialise_matrix_basic);
  tcase_add_test(tc, test_initialise_matrix_zero_size);

  suite_add_tcase(s, tc);
  return s;
}