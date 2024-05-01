TEST_FILES = $(wildcard test/*.cpp)
BUILD_DIR = build

CC  = gcc
CXX = g++
OBJDUMP = objdump
CFLAGS = -Wall -Wextra -Werror -g3 -ggdb -Iinc -std=gnu11 -ffunction-sections -fdata-sections

$(BUILD_DIR)/main: build/main.o | $(BUILD_DIR) Makefile
	$(CC) $(CFLAGS) -o $@ $(BUILD_DIR)/main.o

$(BUILD_DIR)/%.lst: $(BUILD_DIR)/%.o
	$(OBJDUMP) -hDlsw $^ > $@

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR) Makefile
	$(CC) $(CFLAGS) -o $@ -c $^
	./patch.py $@

$(BUILD_DIR)/test: $(TEST_FILES) | $(BUILD_DIR) Makefile
	$(CXX) $(CFLAGS) -Wno-error -Wno-missing-field-initializers -lgtest -lgtest_main \
		$^ \
		-o $@

run_tests: $(BUILD_DIR)/test
	./$(BUILD_DIR)/test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

example: $(BUILD_DIR)/main ## Build the example

run: $(BUILD_DIR)/main ## Run the example program
	./$(BUILD_DIR)/main | python decoder.py dict.txt

clean: ## Clean the build artifacts
	rm -rf $(BUILD_DIR)

test: run_tests ## Run the tests (requires gtest installed)

.PHONY: example clean test run_tests help

help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

.DEFAULT_GOAL := example
