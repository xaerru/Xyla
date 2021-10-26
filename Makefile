CC=gcc
TARGET=build/xyla
CFLAGS=-Wall -Werror -Ofast -Isrc/include

.PHONY: default all clean

default: $(TARGET)
all: default

SOURCES=$(shell find src/xyla -name '*.c')
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	mkdir -p build/
	$(CC) $(OBJECTS) -Wall -o $@

remake: clean all

clean:
	rm -rf build/
	rm -f src/xyla/*.o
