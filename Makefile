CC=gcc
CFLAGS=-c -Wall -Wextra -Wrestrict -pedantic -std=c2x -march=native -Iinclude/ 
LDFLAGS=
OBJ=build/entry.o build/bootstrap.o build/logger.o build/node.o build/stun.o build/host.o build/random.o
EXEC=build/client

.PHONY: all clean run mkdir

all: $(EXECUTABLE) clean mkdir

mkdir:
	mkdir -p build/

build/entry.o: src/main.c
	$(CC) $(CFLAGS) src/main.c -o build/entry.o

build/bootstrap.o: src/bootstrap.c
	$(CC) $(CFLAGS) src/bootstrap.c -o build/bootstrap.o

build/logger.o: src/logger/logger.c
	$(CC) $(CFLAGS) src/logger/logger.c -o build/logger.o

build/random.o: src/crypto/random.c
	$(CC) $(CFLAGS) src/crypto/random.c -o build/random.o

build/stun.o: src/net/stun.c
	$(CC) $(CFLAGS) src/net/stun.c -o build/stun.o

build/host.o: src/net/host.c
	$(CC) $(CFLAGS) src/net/host.c -o build/host.o

build/node.o: src/net/node.c
	$(CC) $(CFLAGS) src/net/node.c -o build/node.o

$(EXEC): mkdir $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

filelog:
	$(MAKE) CFLAGS="$(CFLAGS)-DLOGFILE=\\\"DEBUG.log\\\"" build

build: $(EXEC)

run: 
	./$(EXEC)

clean:
	rm -f build/*
