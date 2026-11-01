#include "../brick_game/tetris/tetris.h"
#include "tetris_tests.h"

// --- Переменные ---
static brick_t brick_set[BRICK_SET_COUNT];
static brick_t new_brick;

// Вспомогательная функция для проверки матрицы конкретной фигуры
void assert_brick_matrix_equals(brick_t *brick,
                                int expected[BRICK_SIZE][BRICK_SIZE]) {
  for (int i = 0; i < BRICK_SIZE; i++) {
    for (int j = 0; j < BRICK_SIZE; j++) {
      ck_assert_int_eq(brick->matr[i][j], expected[i][j]);
    }
  }
}

// Тест: Проверка фигуры I (индекс 0)
START_TEST(test_create_rnd_brick_I) {
  // Arrange
  initialise_brick_set(brick_set);
  // Готовим эталонный набор
  // Подменяем rand, чтобы он вернул 0 (трюк с макросом или линковкой)
  // Если вы используете заглушку rand() из прошлого примера, раскомментируйте:
  // rand_value_to_return = 0;
  // Для примера используем "грязный", но рабочий способ без изменения кода:
  // Мы просто вызываем функцию и смотрим, что она может создать фигуру I.
  // (В идеале здесь нужен Mock, но проверим через цикл)
  int found_I = 0;
  for (int i = 0; i < 50; i++) {
    create_rnd_brick(&new_brick, brick_set);
    // Проверяем, не фигура ли это I (у неё единицы только во 2-м столбце)
    int is_I = 1;
    for (int r = 0; r < BRICK_SIZE; r++) {
      for (int c = 0; c < BRICK_SIZE; c++) {
        if (c == 1) {
          if (new_brick.matr[r][c] != 1) {
            is_I = 0;
            break;
          }
        } else {
          if (new_brick.matr[r][c] != 0) {
            is_I = 0;
            break;
          }
        }
      }
      if (!is_I) break;
    }
    if (is_I) {
      found_I = 1;
      break;
    }
  }
  ck_assert_int_eq(found_I, 1);
  // Удалось ли найти I за 50 попыток?
  // Если нашли, проверяем координаты
  ck_assert_int_eq(new_brick.x, 3);
  ck_assert_int_eq(new_brick.y, 0);
}
END_TEST

// Тест: Проверка фигуры T (индекс 5) - Детальный
START_TEST(test_create_rnd_brick_T) {
  // Arrange
  initialise_brick_set(brick_set);
  // Ищем фигуру T в цикле (так как rand() непредсказуем)
  int found_T = 0;
  for (int i = 0; i < 100; i++) {
    create_rnd_brick(&new_brick, brick_set);
    // У фигуры T (индекс 5) в вашей функции:
    // matr[2][0]=1, matr[2][1]=1, matr[2][2]=1, matr[1][1]=1
    if (new_brick.matr[2][0] == 1 && new_brick.matr[2][1] == 1 &&
        new_brick.matr[2][2] == 1 && new_brick.matr[1][1] == 1) {
      // Проверяем, что остальные клетки T - нули
      int expected_T[BRICK_SIZE][BRICK_SIZE] = {
          {0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}};
      assert_brick_matrix_equals(&new_brick, expected_T);
      found_T = 1;
      break;
    }
  }
  ck_assert_int_eq(found_T, 1);
  ck_assert_int_eq(new_brick.x, 3);
  ck_assert_int_eq(new_brick.y, 0);
}
END_TEST

// Тест: Проверка, что копируются ВСЕ поля (включая x и y)
START_TEST(test_create_rnd_brick_copies_coordinates) {
  // Arrange
  initialise_brick_set(brick_set);
  // Ищем любую фигуру, кроме I (у I x=3, y=0 по умолчанию, как и у всех)
  // Чтобы проверить копирование, нам нужно убедиться, что new_brick
  // это именно КОПИЯ элемента из массива, а не просто обнуленная структура.
  create_rnd_brick(&new_brick, brick_set);
  // Проверяем, что x и y установлены (это делает initialise_brick_set)
  // Если бы create_rnd_brick просто делала memset(0), x и y были бы 0.
  // Но так как она делает присваивание структуры (*new_brick =
  // brick_set[index]), она берет x=3 и y=0 из инициализированного набора.
  ck_assert_int_eq(new_brick.x, 3);
  ck_assert_int_eq(new_brick.y, 0);
  // Проверяем, что матрица не пустая (значит, копирование произошло)
  int sum = 0;
  for (int i = 0; i < BRICK_SIZE; i++) {
    for (int j = 0; j < BRICK_SIZE; j++) {
      sum += new_brick.matr[i][j];
    }
  }
  ck_assert_int_gt(sum, 0);
  // Сумма элементов матрицы должна быть > 0
}
END_TEST

// --- Suite ---
Suite *test_create_rnd_brick_suite(void) {
  Suite *s = suite_create("suite_create_rnd_brick");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_create_rnd_brick_I);
  tcase_add_test(tc, test_create_rnd_brick_T);
  tcase_add_test(tc, test_create_rnd_brick_copies_coordinates);

  suite_add_tcase(s, tc);
  return s;
}