CC=gcc
CFLAGS=-c -Wall -Wextra -Wrestrict -pedantic -std=c2x -march=native -Isrc/ 
LDFLAGS=
OBJ=build/main.o build/node.o build/bootstrap.o build/dispatcher.o build/handler_punch.o build/handler_ping.o build/handler_data.o build/handler_close.o build/udp_socket.o build/hole_punch.o build/ping.o build/addr.o build/stun_client.o build/peer_registry.o build/peer_state.o build/ui.o build/logger.o build/random.o
EXEC=build/client

.PHONY: all clean run mkdir

all: $(EXECUTABLE) clean mkdir

mkdir:
	mkdir -p build/

build/main.o: src/main.c
	$(CC) $(CFLAGS) src/main.c -o build/main.o

build/node.o: src/core/node.c
	$(CC) $(CFLAGS) src/core/node.c -o build/node.o

build/bootstrap.o: src/core/bootstrap.c
	$(CC) $(CFLAGS) src/core/bootstrap.c -o build/bootstrap.o

build/logger.o: src/logger/logger.c
	$(CC) $(CFLAGS) src/logger/logger.c -o build/logger.o

build/random.o: src/crypto/random.c
	$(CC) $(CFLAGS) src/crypto/random.c -o build/random.o

build/stun_client.o: src/net/stun_client.c
	$(CC) $(CFLAGS) src/net/stun_client.c -o build/stun_client.o

build/addr.o: src/net/addr.c
	$(CC) $(CFLAGS) src/net/addr.c -o build/addr.o

build/hole_punch.o: src/net/hole_punch.c 
	$(CC) $(CFLAGS) src/net/hole_punch.c -o build/hole_punch.o

build/ping.o: src/net/ping.c 
	$(CC) $(CFLAGS) src/net/ping.c -o build/ping.o

build/udp_socket.o: src/net/udp_socket.c
	$(CC) $(CFLAGS) src/net/udp_socket.c -o build/udp_socket.o

build/peer_registry.o: src/state/peer_registry.c
	$(CC) $(CFLAGS) src/state/peer_registry.c -o build/peer_registry.o

build/peer_state.o: src/state/peer_state.c
	$(CC) $(CFLAGS) src/state/peer_state.c -o build/peer_state.o

build/ui.o: src/ui/chat_input.c
	$(CC) $(CFLAGS) src/ui/chat_input.c -o build/ui.o

build/dispatcher.o: src/dispatch/dispatcher.c 
	$(CC) $(CFLAGS) src/dispatch/dispatcher.c -o build/dispatcher.o

build/handler_punch.o: src/dispatch/handlers/handler_punch.c 
	$(CC) $(CFLAGS) src/dispatch/handlers/handler_punch.c -o build/handler_punch.o

build/handler_ping.o: src/dispatch/handlers/handler_ping.c 
	$(CC) $(CFLAGS) src/dispatch/handlers/handler_ping.c -o build/handler_ping.o

build/handler_data.o: src/dispatch/handlers/handler_data.c 
	$(CC) $(CFLAGS) src/dispatch/handlers/handler_data.c -o build/handler_data.o

build/handler_close.o: src/dispatch/handlers/handler_close.c
	$(CC) $(CFLAGS) src/dispatch/handlers/handler_close.c -o build/handler_close.o

$(EXEC): mkdir $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(EXEC)

filelog:
	$(MAKE) CFLAGS="$(CFLAGS)-DLOGFILE=\\\"DEBUG.log\\\"" build

debug:
	$(MAKE) CFLAGS="$(CFLAGS)-DDEBUG" build

build: $(EXEC)

run: 
	./$(EXEC)

clean:
	rm -f build/*
