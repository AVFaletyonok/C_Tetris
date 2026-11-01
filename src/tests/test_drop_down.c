#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

static int game_map[ROWS_MAP][COLS_MAP];
static brick_t brick;

void drop_down_setup(void) {
  memset(game_map, 0, sizeof(game_map));
  memset(&brick, 0, sizeof(brick_t));
  brick.x = 3;
  brick.y = 0;
  for (int i = 0; i < BRICK_SIZE; i++) {
    brick.matr[i][1] = 1;
  }
}

START_TEST(test_drop_down) {
  // Arrange:
  tetris_state state = MOVING;
  // Act
  drop_down(&state, game_map, &brick);
  // Assert:
  ck_assert_int_eq(brick.y, 16);
  ck_assert_int_eq(state, ATTACHING);
}
END_TEST

// --- Suite ---
Suite *test_drop_down_suite(void) {
  Suite *s = suite_create("suite_drop_down");
  TCase *tc = tcase_create("Core");

  tcase_add_checked_fixture(tc, drop_down_setup, NULL);

  tcase_add_test(tc, test_drop_down);

  suite_add_tcase(s, tc);
  return s;
}