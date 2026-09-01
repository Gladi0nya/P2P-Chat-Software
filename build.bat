set INCLUDE_DIRS=/Isrc/ /I"C:\\dev\\MSVC\\14.51.36231\\include"
set OBJECTS=build/main.o build/node.o build/bootstrap.o build/stun_client.o build/udp_socket.o build/addr.o build/hole_punch.o build/peer_registry.o build/random.o build/dispatcher.o build/handler_close.o build/handler_data.o build/handler_punch.o build/chat_input.o build/logger.o

cl /c src/main.c %INCLUDE_DIRS% /Fobuild/main.o
cl /c src/core/node.c %INCLUDE_DIRS% /Fobuild/node.o
cl /c src/core/bootstrap.c %INCLUDE_DIRS% /Fobuild/bootstrap.o
cl /c src/net/stun_client.c %INCLUDE_DIRS% /Fobuild/stun_client.o
cl /c src/net/udp_socket.c %INCLUDE_DIRS% /Fobuild/udp_socket.o
cl /c src/net/addr.c %INCLUDE_DIRS% /Fobuild/addr.o
cl /c src/net/hole_punch.c %INCLUDE_DIRS% /Fobuild/hole_punch.o
cl /c src/state/peer_registry.c %INCLUDE_DIRS% /Fobuild/peer_registry.o
cl /c src/crypto/random.c %INCLUDE_DIRS% /Fobuild/random.o
cl /c src/dispatch/dispatcher.c %INCLUDE_DIRS% /Fobuild/dispatcher.o
cl /c src/dispatch/handlers/handler_close.c %INCLUDE_DIRS% /Fobuild/handler_close.o
cl /c src/dispatch/handlers/handler_data.c %INCLUDE_DIRS% /Fobuild/handler_data.o
cl /c src/dispatch/handlers/handler_punch.c %INCLUDE_DIRS% /Fobuild/handler_punch.o
cl /c src/ui/chat_input.c %INCLUDE_DIRS% /Fobuild/chat_input.o
cl /c /DDEBUG src/logger/logger.c %INCLUDE_DIRS% /Fobuild/logger.o

ld %OBJECTS% /OUT:client.exe

del build/*.o
