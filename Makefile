CC = gcc
CFLAGS = -Wall -Wextra -g
SRC_DIR = src
TARGET = chash

# List all the source files
SRC_FILES = chash.c $(SRC_DIR)/hash.c $(SRC_DIR)/hash_functions.c $(SRC_DIR)/rwlock.c

# The default rule: compile and link everything into the executable
$(TARGET): $(SRC_FILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC_FILES)

# Clean up the generated files
clean:
	rm -f $(TARGET)