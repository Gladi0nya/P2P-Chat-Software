CC=gcc
CFLAGS=-c -Wall -Wextra -Wrestrict -Iinclude/ 
LDFLAGS=
OBJ=build/entry.o build/bootstrap.o build/logger.o build/node.o
EXEC=build/client

.PHONY: all clean run mkdir

all: $(EXECUTABLE) clean mkdir

mkdir:
	mkdir -p build/
	mkdir -p results/	

build/entry.o:
	$(CC) $(CFLAGS) src/main.c -o build/entry.o

build/bootstrap.o:
	$(CC) $(CFLAGS) src/bootstrap.c -o build/bootstrap.o

build/logger.o:
	$(CC) $(CFLAGS) src/logger/logger.c -o build/logger.o

build/node.o:
	$(CC) $(CFLAGS) src/net/node.c -o build/node.o

$(EXEC): mkdir $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

filelog:
	$(MAKE) CFLAGS="$(CFLAGS)-DLOGFILE=\\\"DEBUG.log\\\"" build

build: $(EXEC)
	rm -f build/*.o

run: 
	./$(EXEC)

clean:
	rm -f build/*
