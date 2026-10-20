CC := gcc

SRC := ./src
INCLUDE := ./include
BUILD := ./build
TARGET := $(BUILD)/game

SRCS := $(shell find $(SRC) -name '*.c')
OBJS := $(SRCS:%=$(BUILD)/%.o)
DEPS := $(OBJS:.o=.d)

IDIRS := $(shell find $(INCLUDE) -type d)
IFLAGS := $(addprefix -I, $(IDIRS))

CPPFLAGS := $(IFLAGS) -MMD -MP -D_POSIX_C_SOURCE=200809L
CFLAGS := -Wall -Wextra -std=c11 -g -O0 -fno-omit-frame-pointer
LDLIBS := -lm

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p $(dir $@)
	$(CC) $(OBJS) $(LDLIBS) -o $@

$(BUILD)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)

-include $(DEPS)
