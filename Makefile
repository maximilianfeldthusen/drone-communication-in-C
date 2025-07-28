
# Drone communication Makefile
CC = gcc
CFLAGS = -Wall -I./c_library_v2/common
SRC = drone_comm.c
BIN = drone_comm

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

clean:
	rm -f $(BIN)
