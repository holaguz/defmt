TEST_FILES = $(wildcard test/*.cpp)
BUILD_DIR = build

CC  = gcc
CXX = g++
CFLAGS = -Wall -Wextra -Werror -g3 -Iinc

build/main: build/main.o | $(BUILD_DIR) Makefile
	$(CC) $(CFLAGS) -o $@ build/main.o

build/%.o: src/%.c | $(BUILD_DIR) Makefile
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	rm -rf build *.o

build/test: $(TEST_FILES) | $(BUILD_DIR) Makefile
	$(CXX) $(CFLAGS) -Wno-error -Wno-missing-field-initializers -lgtest -lgtest_main \
		$^ \
		-o $@

run_tests: build/test
	./build/test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

test: run_tests

default: build/main build/test
.PHONY: clean test run_tests
