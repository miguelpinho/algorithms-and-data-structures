# Auto-dependency generation based on:
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation

# Compiler flags
CC      := gcc
CFLAGS  := -O3 -Wall -ansi
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d

# Target binary
TARGET  := autopark

# Folders
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := obj
DEP_DIR := d

# Files
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPENDS := $(SOURCES:$(SRC_DIR)/%.c=$(DEP_DIR)/%.d)

# Rules
.PHONY: all clean fullclean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEP_DIR)/%.d | $(OBJ_DIR) $(DEP_DIR)
	$(CC) -c $(DEPFLAGS) $(CFLAGS) -I$(INC_DIR) $< -o $@ 

$(DEPENDS):

$(OBJ_DIR) $(DEP_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(DEP_DIR)

fullclean: clean
	rm -f autopark

# Dependencies
include $(wildcard $(DEPENDS))