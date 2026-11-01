#include <check.h>
#include <stdio.h>

#include "../brick_game/tetris/tetris.h"

Suite* test_check_lines(void);
Suite* test_check_intersection(void);
Suite* test_move_down(void);
Suite* test_change_speed(void);
Suite* test_user_input(void);
Suite* test_initialise_matrix_suite(void);
Suite* test_initialise_brick_set_suite(void);
Suite* test_create_rnd_brick_suite(void);
Suite* test_read_save_high_score_suite(void);
Suite* test_bake_brick_suite(void);
Suite* test_move_right_suite(void);
Suite* test_move_left_suite(void);
Suite* test_rotate_suite(void);
Suite* test_drop_down_suite(void);
Suite* test_gameover_suite(void);
Suite* test_attaching_state_suite(void);
Suite* test_shifting_state_suite(void);
Suite* test_moving_state_suite(void);
Suite* test_spawn_state_suite(void);
Suite* test_pause_state_suite(void);
Suite* test_start_state_suite(void);