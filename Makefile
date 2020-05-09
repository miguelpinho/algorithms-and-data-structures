# Compiler flags
CC      := gcc
CFLAGS  := -O3 -Wall -ansi

# Target binary
TARGET  := autopark

# Folders
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := obj

# Files
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Rules
.PHONY: all dir clean fullclean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $(OBJECTS)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c | dir
	$(CC) -c $(CFLAGS) -I$(INC_DIR) $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | dir
	$(CC) -c $(CFLAGS) -I$(INC_DIR) $< -o $@ 

dir:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fullclean: clean
	rm -f autopark