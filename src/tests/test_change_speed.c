#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

static GameInfo_t game_info;
static brick_t brick;

START_TEST(test_change_speed_levels) {
  game_info.score = SCORE_TO_SPEED_UP * 2;  // 2 уровня пройдено
  game_info.speed = START_SPEED;

  change_speed(&game_info);

  // Скорость должна быть START_SPEED * 0.9 * 0.9
  int expected = (int)(START_SPEED * 0.81);
  ck_assert_int_eq(game_info.speed, expected);
}
END_TEST

Suite* test_change_speed(void) {
  Suite* s = suite_create("suite_change_speed");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_change_speed_levels);

  suite_add_tcase(s, tc);
  return s;
}