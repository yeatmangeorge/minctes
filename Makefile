# ===== User Config =====
CC ?= clang
BUILD_DIR := build
SRC_DIR := src
TARGET := minctes

# Set DEBUG=1 for debugging, SAN=1 for sanitizers
DEBUG ?= 1
SAN ?= 1

# ===== Compiler Standard & Warnings =====
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

# ===== Optimization and Debug Flags =====
OPT_FLAGS := -O2
DEBUG_FLAGS := -g3
SAN_FLAGS :=
SAN_LDFLAGS :=

ifeq ($(DEBUG),1)
	# Debug build: no optimization
	OPT_FLAGS := -O0
	DEBUG_FLAGS := -g3
endif

ifeq ($(SAN),1)
	# Enable ASan/UBSan if requested
	SAN_FLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
	SAN_LDFLAGS += -fsanitize=address,undefined
	# Reduce optimization for sanitizer builds
	ifeq ($(DEBUG),0)
		OPT_FLAGS := -O1
	endif
endif

CFLAGS := -std=$(C_STANDARD) $(COMMON_WARNINGS) $(OPT_FLAGS) $(DEBUG_FLAGS) $(SAN_FLAGS)

# ===== Source Files =====
SRCS := $(shell find $(SRC_DIR) -name '*.c' ! -name '*.t.c')
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
	clang-format -i $(shell find $(SRC_DIR) -name '*.[ch]')

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

.PHONY: all clean rebuild fmt