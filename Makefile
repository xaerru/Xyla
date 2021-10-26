CC:=gcc
CFLAGS:=-Wall -Werror -std=gnu99 -Ofast -Isrc/include

TARGET:=build/bin/xyla
BUILD_DIR:=build

SOURCES:=$(shell find src/ -type f -name '*.c')
OBJECTS:= $(patsubst %.c,build/%.o, $(SOURCES))
HEADERS:= $(shell find src/ -type f -name '*.h')

.PHONY: default all clean remake clean
.PRECIOUS: $(TARGET) $(OBJECTS)

default: $(TARGET)
all: default

$(BUILD_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)/bin
	$(CC) $(OBJECTS) -Wall -o $@

remake: clean all

clean:
	rm -rf $(BUILD_DIR)
