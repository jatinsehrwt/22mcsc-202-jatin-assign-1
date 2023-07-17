# Makefile for the copy_file program

# The target
TARGET = ./bin/j29

# The source files
SRC = $(wildcard src/modules/*.c)
MAIN = ./src/main.c

# The header files
HDR = $(wildcard src/include/*.h)

# The object files
OBJ = $(SRC:.c=.o)
OBJ += $(MAIN:.c=.o)

BUILD_DIR := ./build

# The compiler
CC = gcc

# The compiler flags
CFLAGS = -Wall -Wextra -g

# The linker flags
LDFLAGS = -lpthread

# The target build rule
$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

# The object file build rules
%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

# The clean target
.PHONY: clean
clean:
	rm -f $(OBJ)

