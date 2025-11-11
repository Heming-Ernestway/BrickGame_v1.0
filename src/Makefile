CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
BUILD_DIR = ./build
BACKEND = ./brick_game/tetris/backend.c
FRONTEND = ./gui/cli/frontend.c
TETRIS_SRC = ./brick_game/tetris/backend.c tetris.c
TESTS_SRC = ./tests/tetris_tests.c
OS := $(shell uname)

ifeq ($(OS), Linux)
	LIBS = -lcheck -lncursesw -lrt -lpthread -lsubunit -lm
else
	LIBS = -lncursesw -I$(shell brew --prefix check)/include
endif

all: install

$(BUILD_DIR)/tetris: $(TETRIS_SRC) $(FRONTEND)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/tetris $(TETRIS_SRC) $(FRONTEND) $(LIBS)
	

install: $(BUILD_DIR)/tetris

uninstall:
	rm -rf ./build

test: clean
	$(CC) $(CFLAGS) $(BACKEND) $(FRONTEND) $(TESTS_SRC) -o test $(LIBS)
	rm -f *.o
	./test

gcov_report: test
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage $(BACKEND) $(FRONTEND) $(TESTS_SRC) -o test $(LIBS)
	./test
	lcov -t "test_s21_tetris" -o tetris.info -c -d .
	genhtml -o report tetris.info
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.txt
	rm -f test
	open report/index.html

dvi:
	doxygen .Doxyfile
	open documentation/html/index.html

dist:
	tar -czf tetris.tar.gz build/tetris

clean:
	rm -f *.o *.txt
	rm -f *.tar.gz
	rm -f test
	rm -f *.info
	rm -f *.gcda
	rm -f *.gcno
	rm -rf report
	rm -rf documentation

style:
	clang-format -i $(TETRIS_SRC) $(FRONTEND) $(TESTS_SRC) ./tests/tetris_tests.h tetris.h