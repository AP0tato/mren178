CC ?= $(shell which gcc || which clang || which g++)
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -g -I src/inc
BUILD := build_make
TARGET_GEN := $(BUILD)/makeidnt
TARGET_LAB := $(BUILD)/lab

.PHONY: clean gen_file lab

clean:
	rm -rf $(BUILD)

gen_file: $(TARGET_GEN)
	./$(TARGET_GEN)

lab: $(TARGET_LAB)
	./$(TARGET_LAB)

$(TARGET_LAB): $(BUILD) lab3_2.c
	$(CC) $(CFLAGS) lab3_2.c -o $@

$(TARGET_GEN): $(BUILD) MAKEIDNT.c
	$(CC) $(CFLAGS) MAKEIDNT.c -o $@

$(BUILD):
	mkdir -p $(BUILD)