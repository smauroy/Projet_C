
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g

TARGET = build/projet_c
SRC = main.c

.PHONY: all run check test clean

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ $<

run: all
	./$(TARGET)

check:
	$(CC) $(CFLAGS) -fsyntax-only $(SRC)

test: check
	@echo "Syntax check passed (unit tests not yet implemented)"

clean:
	rm -rf build
