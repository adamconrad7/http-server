CC = clang
CFLAGS = -Wall -Wextra -I./src -I./tests/unity -v
LDFLAGS = 

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

SERVER_SOURCES = $(wildcard $(SRC_DIR)/*.c)
SERVER_OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SERVER_SOURCES))

TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SOURCES))

UNITY_SOURCE = $(TEST_DIR)/unity/unity.c
UNITY_OBJECT = $(BUILD_DIR)/unity.o

.PHONY: all clean test

all: $(BUILD_DIR)/server $(BUILD_DIR)/test_runner

$(BUILD_DIR)/server: $(SERVER_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/test_runner: $(TEST_OBJECTS) $(UNITY_OBJECT) $(filter-out $(BUILD_DIR)/main.o,$(SERVER_OBJECTS))
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(UNITY_OBJECT): $(UNITY_SOURCE) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

test: $(BUILD_DIR)/test_runner
	./$(BUILD_DIR)/test_runner
