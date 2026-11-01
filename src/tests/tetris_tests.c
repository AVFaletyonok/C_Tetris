#include "tetris_tests.h"

int main() {
  Suite* s[] = {test_check_lines(),
                test_check_intersection(),
                test_move_down(),
                test_change_speed(),
                test_user_input(),
                test_initialise_matrix_suite(),
                test_initialise_brick_set_suite(),
                test_create_rnd_brick_suite(),
                test_read_save_high_score_suite(),
                test_bake_brick_suite(),
                test_move_right_suite(),
                test_move_left_suite(),
                test_rotate_suite(),
                test_drop_down_suite(),
                test_gameover_suite(),
                test_attaching_state_suite(),
                test_shifting_state_suite(),
                test_moving_state_suite(),
                test_spawn_state_suite(),
                test_pause_state_suite(),
                test_start_state_suite(),
                NULL};

  printf("\nTESTS MODULE\n\n");
  SRunner* sr;
  sr = srunner_create(s[0]);
  for (int i = 1; s[i] != NULL; i++) srunner_add_suite(sr, s[i]);

  srunner_set_fork_status(sr, CK_FORK);
  srunner_run_all(sr, CK_NORMAL);

  srunner_free(sr);

  return 0;
}