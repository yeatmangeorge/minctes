CC ?= clang
SRC_DIR := src
TARGET := minctes
LIB_TARGET := lib$(TARGET).a
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
BASE_CFLAGS := -std=$(C_STANDARD) $(COMMON_WARNINGS)
SRCS := $(shell find $(SRC_DIR) -name '*.c' ! -name '*.t.c')

CONFIG ?= debug
ifeq ($(CONFIG),release)
	BUILD_DIR := build/release
	CFLAGS := $(BASE_CFLAGS) -O2
	LDFLAGS :=
endif
ifeq ($(CONFIG),debug)
	BUILD_DIR := build/debug
	CFLAGS := $(BASE_CFLAGS) -O0 -g3 -DDEBUG \
	          -fsanitize=address,undefined -fno-omit-frame-pointer
	LDFLAGS := -fsanitize=address,undefined
endif

# Library always builds in release mode regardless of CONFIG
LIB_BUILD_DIR := build/release
LIB_CFLAGS    := $(BASE_CFLAGS) -O2
LIB_OBJS      := $(SRCS:$(SRC_DIR)/%.c=$(LIB_BUILD_DIR)/%.o)

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# ==========================================================
# Targets
# ==========================================================
all: $(CONFIG)
release:
	@$(MAKE) CONFIG=release build
debug:
	@$(MAKE) CONFIG=debug build
build: fmt $(BUILD_DIR)/$(TARGET)
$(BUILD_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

lib: fmt $(LIB_BUILD_DIR)/$(LIB_TARGET)
$(LIB_BUILD_DIR)/$(LIB_TARGET): $(LIB_OBJS)
	@mkdir -p $(dir $@)
	ar rcs $@ $(LIB_OBJS)
	cp $(SRC_DIR)/*.h $(LIB_BUILD_DIR)/
	cp $(SRC_DIR)/*.x $(LIB_BUILD_DIR)/

$(LIB_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(LIB_CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

fmt:
	clang-format -i $(shell find $(SRC_DIR) -name '*.[ch]')
clean:
	rm -rf build
rebuild: clean all
.PHONY: all release debug build lib clean rebuild fmt