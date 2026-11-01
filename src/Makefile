CC = gcc
CFLAGS = -std=c11 #-Wall -Werror -Wextra
UBUNTU_FLAGS = -lsubunit -lrt -lpthread -lm
LIBS = -lncursesw
TEST_LIBS = -lcheck
LDFLAGS =
LIB_PATH = brick_game/tetris
GUI_PATH = gui/cli
SOURCES = $(TETRIS_SOURCES) $(FRONTEND_SOURCES)
TETRIS_SOURCES = $(LIB_PATH)/tetris.c
FRONTEND_SOURCES = $(GUI_PATH)/frontend.c
OBJECTS = $(TETRIS_OBJECT) $(FRONTEND_OBJECT)
TETRIS_OBJECT = $(BUILD_DIR)/tetris.o
FRONTEND_OBJECT = $(BUILD_DIR)/frontend.o

BUILD_DIR = build
EXECUTABLE = $(BUILD_DIR)/tetris

DATA_SRC = data
DATA_DST = $(BUILD_DIR)/$(DATA_SRC)

TEST_DIR = tests
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)

DIST_NAME = tetris-dist
OS=$(shell uname -s)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(DATA_DST)

$(TEST_OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: test

test: install $(TEST_OBJECTS)
ifeq ($(OS), Darwin)
	$(CC) -o unit_test $(TETRIS_OBJECT) $(TEST_OBJECTS) $(TEST_LIBS)
else
	$(CC) -o unit_test $(TETRIS_OBJECT) $(TEST_OBJECTS) $(TEST_LIBS) $(UBUNTU_FLAGS)
endif
	./unit_test
	rm -rf unit_test $(TEST_DIR)/*.o

install: $(SOURCES) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(TETRIS_SOURCES) -o $(TETRIS_OBJECT)
	$(CC) $(CFLAGS) -c $(FRONTEND_SOURCES) -o $(FRONTEND_OBJECT)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE) $(LIBS)
	cp -rf $(DATA_SRC)/* $(DATA_DST)/

debug: $(SOURCES)
	$(CC) $(CFLAGS) -g -O0 $(SOURCES) -o tetris_debug $(LIBS)

# if you need to create TeX/LaTeX documentation .ps or .pdf
dvi:

dist:
	rm -rf $(DIST_NAME)
	mkdir -p $(DIST_NAME)
	cp -rf Makefile ../README.md brick_game gui data images tests $(DIST_NAME)/
	zip -r $(DIST_NAME).zip $(DIST_NAME)
	rm -rf $(DIST_NAME)
	@echo "Created $(DIST_NAME).zip"

clang-format:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n brick_game/tetris/*.c brick_game/tetris/*.h gui/cli/*.c gui/cli/*.h tests/*.c tests/*.h
	rm .clang-format

cppcheck:
	cppcheck --enable=all --std=c11 --check-level=exhaustive --disable=information \
	--suppress=missingIncludeSystem --suppress=missingInclude --suppress=checkersReport ./

uninstall:
	rm -rf build

clean:
	rm -rf $(BUILD_DIR)/*.o gcov_test gcov_test.info *.gc* report tetris-dist.zip

gcov_report:
ifeq ($(OS), Darwin)
	$(CC) --coverage $(CFLAGS) $(TEST_SOURCES) $(TETRIS_SOURCES) -o gcov_test $(TEST_LIBS)
else
	$(CC) --coverage $(CFLAGS) $(TEST_SOURCES) $(TETRIS_SOURCES) -o gcov_test $(TEST_LIBS) $(UBUNTU_FLAGS)
endif
	./gcov_test
	lcov -t "gcov_test" -o gcov_test.info -c -d .
	genhtml -o report gcov_test.info
ifeq ($(OS), Darwin)
	open ./report/index.html
else
	-xdg-open ./report/index.html
endif
	rm -rf *.gc* gcov_test gcov_test.info unit_test