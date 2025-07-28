
# Drone communication Makefile
CC = gcc
CFLAGS = -Wall -I./c_library_v2/common
SRC = drone_com.c
BIN = drone_com

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

clean:
	rm -f $(BIN)
