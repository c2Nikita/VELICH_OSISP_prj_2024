TARGET = SimpleTetris
CC = gcc

PREF_SRC = ./src/
PREF_OBJ = ./obj/

SRC = $(shell find . -name "*.c")
OBJ = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRC))

$(TARGET) : $(OBJ)
	$(CC) $(OBJ) -lncurses -o $(TARGET)

$(PREF_OBJ)%.o : $(PREF_SRC)%.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@

clean:
	rm $(TARGET) $(OBJ)