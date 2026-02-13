# ===== User Config =====
CC ?= clang
BUILD_DIR := build
SRC_DIR := src
INC_DIR := include
TARGET := minctes
SAN ?= 0

# ===== Compiler Flags =====
C_STANDARD := c99

COMMON_WARNINGS := \
	-Wall \
	-Wextra \
	-Wpedantic \
	-Werror \
	-Wshadow \
	-Wconversion \
	-Wdouble-promotion \
	-Wformat=2 \
	-Wundef \
	-Wnull-dereference \
	-Wimplicit-fallthrough

OPT_FLAGS := -O2
DEBUG_FLAGS := -g3

SAN_FLAGS :=
SAN_LDFLAGS :=

ifeq ($(SAN),1)
SAN_FLAGS += \
	-fsanitize=address,undefined \
	-fno-omit-frame-pointer \
	-O1

SAN_LDFLAGS += \
	-fsanitize=address,undefined
endif

CFLAGS := \
	-std=$(C_STANDARD) \
	$(COMMON_WARNINGS) \
	$(OPT_FLAGS) \
	$(DEBUG_FLAGS) \
	$(SAN_FLAGS) \
	-I$(INC_DIR)

# ===== Sources =====
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# ===== Targets =====
all: fmt $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(OBJS) -o $@ $(SAN_LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ===== Utilities =====
fmt:
	clang-format -i $(shell find src include -name '*.[ch]')

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

.PHONY: all clean rebuild fmt
