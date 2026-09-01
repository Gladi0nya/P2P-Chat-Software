cl /c src/main.c /Isrc/ /Fobuild/main.o
cl /c src/core/node.c /Isrc/ /Fobuild/node.o
cl /c src/core/bootstrap.c /Isrc/ /Fobuild/bootstrap.o
cl /c src/net/stun_client.c /Isrc/ /Fobuild/stun_client.o
cl /c src/net/udp_socket.c /Isrc/ /Fobuild/udp_socket.o
cl /c src/net/addr.c /Isrc/ /Fobuild/addr.o
cl /c src/net/hole_punch.c /Isrc/ /Fobuild/hole_punch.o
cl /c src/state/peer_registry.c /Isrc/ /Fobuild/peer_registry.o
cl /c src/crypto/random.c /Isrc/ /Fobuild/random.o
cl /c src/dispatch/dispatcher.c /Isrc/ /Fobuild/dispatcher.o
cl /c src/dispatch/handlers/handler_close.c /Isrc/ /Fobuild/handler_close.o
cl /c src/dispatch/handlers/handler_data.c /Isrc/ /Fobuild/handler_data.o
cl /c src/dispatch/handlers/handler_punch.c /Isrc/ /Fobuild/handler_punch.o
cl /c src/ui/chat_input.c /Isrc/ /Fobuild/chat_input.o
cl /c /DDEBUG src/logger/logger.c /Isrc/ /Fobuild/logger.o

ld build/main.o build/node.o build/bootstrap.o build/stun_client.o build/udp_socket.o build/addr.o build/hole_punch.o build/peer_registry.o build/random.o build/dispatcher.o build/handler_close.o build/handler_data.o build/handler_punch.o build/chat_input.o build/logger.o /OUT:client.exe

del build/*.o
