#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

// Тестовые данные
static brick_t brick_set[BRICK_SET_COUNT];
// Вспомогательная функция для проверки матрицы 4x4
void assert_matrix_equals(int actual[BRICK_SIZE][BRICK_SIZE],
                          int expected[BRICK_SIZE][BRICK_SIZE]) {
  for (int i = 0; i < BRICK_SIZE; i++) {
    for (int j = 0; j < BRICK_SIZE; j++) {
      ck_assert_int_eq(actual[i][j], expected[i][j]);
    }
  }
}

// Тест: Проверка начальных координат (x=3, y=0)
START_TEST(test_brick_set_coordinates) {
  initialise_brick_set(brick_set);
  for (int i = 0; i < BRICK_SET_COUNT; i++) {
    ck_assert_int_eq(brick_set[i].x, 3);
    ck_assert_int_eq(brick_set[i].y, 0);
  }
}
END_TEST

// Тест: Фигура I (индекс 0)
START_TEST(test_brick_I) {
  initialise_brick_set(brick_set);
  int expected_I[BRICK_SIZE][BRICK_SIZE] = {
      {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}};
  assert_matrix_equals(brick_set[0].matr, expected_I);
}
END_TEST

// Тест: Фигура J (индекс 1)
START_TEST(test_brick_J) {
  initialise_brick_set(brick_set);
  int expected_J[BRICK_SIZE][BRICK_SIZE] = {
      {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
  assert_matrix_equals(brick_set[1].matr, expected_J);
}
END_TEST

// Тест: Фигура L (индекс 2)
START_TEST(test_brick_L) {
  initialise_brick_set(brick_set);
  int expected_L[BRICK_SIZE][BRICK_SIZE] = {
      {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
  assert_matrix_equals(brick_set[2].matr, expected_L);
}
END_TEST

// Тест: Фигура O (индекс 3)
START_TEST(test_brick_O) {
  initialise_brick_set(brick_set);
  int expected_O[BRICK_SIZE][BRICK_SIZE] = {
      {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
  assert_matrix_equals(brick_set[3].matr, expected_O);
}
END_TEST

// Тест: Фигура S (индекс 4)
START_TEST(test_brick_S) {
  initialise_brick_set(brick_set);
  int expected_S[BRICK_SIZE][BRICK_SIZE] = {
      {0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}};
  assert_matrix_equals(brick_set[4].matr, expected_S);
}
END_TEST

// Тест: Фигура T (индекс 5)
START_TEST(test_brick_T) {
  initialise_brick_set(brick_set);
  int expected_T[BRICK_SIZE][BRICK_SIZE] = {
      {0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}};
  assert_matrix_equals(brick_set[5].matr, expected_T);
}
END_TEST

// Тест: Фигура Z (индекс 6)
START_TEST(test_brick_Z) {
  initialise_brick_set(brick_set);
  int expected_Z[BRICK_SIZE][BRICK_SIZE] = {
      {0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
  assert_matrix_equals(brick_set[6].matr, expected_Z);
}
END_TEST

// --- Suite ---
Suite *test_initialise_brick_set_suite(void) {
  Suite *s = suite_create("suite_initialise_brick_set");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_brick_set_coordinates);
  tcase_add_test(tc, test_brick_I);
  tcase_add_test(tc, test_brick_J);
  tcase_add_test(tc, test_brick_L);
  tcase_add_test(tc, test_brick_O);
  tcase_add_test(tc, test_brick_S);
  tcase_add_test(tc, test_brick_T);
  tcase_add_test(tc, test_brick_Z);

  suite_add_tcase(s, tc);
  return s;
}