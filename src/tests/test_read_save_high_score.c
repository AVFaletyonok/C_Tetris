#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

static char test_dir[] = ".";

// --- Тесты ---
// Тест 1:
START_TEST(test_read_high_score_file_missing) {
  // Arrange:
  save_high_score(0, test_dir);
  // Act:
  int score = read_high_score(test_dir);
  // Assert
  ck_assert_int_eq(score, 0);
}
END_TEST

// Тест 2:
START_TEST(test_read_high_score_valid_number) {
  // Arrange:
  save_high_score(200, test_dir);
  // Act
  int score = read_high_score(test_dir);
  // Assert
  ck_assert_int_eq(score, 200);
}
END_TEST

Suite *test_read_save_high_score_suite(void) {
  Suite *s = suite_create("suite_read/save_high_score");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_read_high_score_file_missing);
  tcase_add_test(tc, test_read_high_score_valid_number);

  suite_add_tcase(s, tc);
  return s;
}