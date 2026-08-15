CC=gcc
CFLAGS=-c -Wall -Wextra -Wrestrict -Iinclude/ 
LDFLAGS=
OBJ=build/client.o build/logger.o
EXEC=build/client

all: $(EXECUTABLE) clean mkdir

mkdir:
	mkdir -p build/
	mkdir -p results/	

build/client.o:
	$(CC) $(CFLAGS) src/main.c -o build/client.o

build/logger.o:
	$(CC) $(CFLAGS) src/logger/logger.c -o build/logger.o

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

run: $(EXEC)
	rm -f build/*.o
	./$(EXEC)

clean:
	rm -f build/*
