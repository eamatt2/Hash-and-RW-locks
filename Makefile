CC = gcc
CFLAGS = -Wall -pthread -I./src
SRC_DIR = src
OBJ_DIR = obj
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/hash.o $(OBJ_DIR)/hash_functions.o $(OBJ_DIR)/rwlock.o

chash: $(OBJS)
	$(CC) $(CFLAGS) -o chash $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) chash output.txt
