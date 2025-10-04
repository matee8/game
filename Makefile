PROJECT_NAME = game
TARGET = $(BUILD_DIR)/$(PROJECT_NAME)

SRC_DIR = src
BIN_DIR = bin
INC_DIR = include
TEST_DIR = tests
BUILD_DIR = build
OBJ_DIR = obj
ASSETS_DIR = assets

CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic --std=c23 -g -I$(INC_DIR) -MMD -MP
LDFLAGS =
LDLIBS =

ifeq ($(OS), Windows_NT)
	LDLIBS += -lraylib -lopengl32 -lgdi32 -lwinmm -lpthread
	TARGET := $(BUILD_DIR)/$(PROJECT_NAME).exe
	CFLAGS += -I"C:/raylib/include"
	LDFLAGS += -L"C:/raylib/lib"
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		LDLIBS += -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	endif
	ifeq ($(UNAME_S), Darwin)
		LDLIBS += -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -Wno-deprecated-declarations
	endif
endif


APP_SRC = $(shell find $(BIN_DIR) -type f -name '*.c')
LIB_SRC = $(shell find $(SRC_DIR) -type f -name '*.c')
TEST_SRC = $(shell find $(TEST_DIR) -type f -name '*.c')

APP_OBJ = $(patsubst $(BIN_DIR)/%.c, $(OBJ_DIR)/%.o, $(APP_SRC))
LIB_OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(LIB_SRC))
ALL_OBJ = $(APP_OBJ) $(LIB_OBJ)
DEPS = $(ALL_OBJ:.o=.d)

TEST_TARGETS = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%, $(TEST_SRC))


CHECK_FILES = $(shell find $(BIN_DIR) $(SRC_DIR) $(TEST_DIR) $(INC_DIR) -name '*.c' -or -name '*.h')

.PHONY: all test clean run check format-check tidy-check format tidy-fix

all: $(TARGET) copy-assets

run: all
	./$(TARGET)

$(TARGET): $(APP_OBJ) $(LIB_OBJ)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

test: $(TEST_TARGETS)
	@for test_exec in $(TEST_TARGETS); do \
		./$$test_exec; \
	done

$(BUILD_DIR)/%: $(TEST_DIR)/%.c $(LIB_OBJ)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(BIN_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@


copy-assets:
	@cp -r $(ASSETS_DIR) $(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(OBJ_DIR)

check: format-check tidy-check

format-check:
	@clang-format --style=file --dry-run -Werror $(CHECK_FILES)

tidy-check:
	@clang-tidy $(CHECK_FILES) -- $(CFLAGS) > /dev/null

format:
	@clang-format -i $(CHECK_FILES)

tidy-fix:
	@clang-tidy -fix $(CHECK_FILES) -- $(CFLAGS) > /dev/null

-include $(DEPS)
