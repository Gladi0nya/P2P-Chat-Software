cl /c src/main.c /Isrc/ /out:build/main.o
cl /c src/core/node.c /Isrc/ /out:build/node.o
cl /c src/core/bootstrap.c /Isrc/ /out:build/bootstrap.o
cl /c src/net/stun_client.c /Isrc/ /out:build/stun_client.o
cl /c src/net/udp_socket.c /Isrc/ /out:build/udp_socket.o
cl /c src/net/addr.c /Isrc/ /out:build/addr.o
cl /c src/net/hole_punch.c /Isrc/ /out:build/hole_punch.o
cl /c src/state/peer_registry.c /Isrc/ /out:build/peer_registry.o
cl /c src/crypto/random.c /Isrc/ /out:build/random.o
cl /c src/dispatch/dispatcher.c /Isrc/ /out:build/dispatcher.o
cl /c src/dispatch/handlers/handler_close.c /Isrc/ /out:build/handler_close.o
cl /c src/dispatch/handlers/handler_data.c /Isrc/ /out:build/handler_data.o
cl /c src/dispatch/handlers/handler_punch.c /Isrc/ /out:build/handler_punch.o
cl /c src/ui/chat_input.c /Isrc/ /out:build/chat_input.o
cl /c /DDEBUG src/logger/logger.c /Isrc/ /out:build/logger.o

ld build/main.o build/node.o build/bootstrap.o build/stun_client.o build/udp_socket.o build/addr.o build/hole_punch.o build/peer_registry.o build/random.o build/dispatcher.o build/handler_close.o build/handler_data.o build/handler_punch.o build/chat_input.o build/logger.o /out:client.exe

del build/*.o
