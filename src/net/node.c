#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include "node.h"
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

#include <pthread.h>

typedef struct {
    int sock;
    struct sockaddr_in peer_addr;
} ThreadArgs;

void* listen_thread(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    char buffer[1024];
    struct sockaddr_in from_addr;
    socklen_t addr_len = sizeof(from_addr);
    
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = recvfrom(args->sock, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr*)&from_addr, &addr_len);
        
        if (n > 0) {
            buffer[n] = '\0';
            printf("\n[Pair] %s\n", buffer);
            printf("[Chat] ");
            fflush(stdout);
        }
    }
    return NULL;
}

uint8_t CreateChannelForPeer(int sock, addr_t peer_addr) {
  if (sock < 0) {
    LOG_ERROR("Invalid socket provided.");

    return 1;
  }
    
  struct sockaddr_in peer = {
    .sin_family = AF_INET,
    .sin_port = htons(peer_addr.port),
    .sin_addr.s_addr = peer_addr.ip
  };
    
    ThreadArgs args = {sock, peer};
    pthread_t listener;
    pthread_create(&listener, NULL, listen_thread, &args);
    
    printf("[*] Hole punching to %u.%u.%u.%u:%u...\n", *(uint8_t*)&peer_addr.ip, *((uint8_t*)&peer_addr.ip + 1), *((uint8_t*)&peer_addr.ip + 2), *((uint8_t*)&peer_addr.ip + 3), peer_addr.port);
    
    for (int i = 0; i < 100; i++) {
        char msg[32];
        snprintf(msg, sizeof(msg), "PING-%d", i);
        sendto(sock, msg, strlen(msg), 0,
               (struct sockaddr*)&peer, sizeof(peer));
        printf("[*] Sent %s\n", msg);

	struct timespec ts = {
	  .tv_sec = 100 / 1000,
	  .tv_nsec = (100 % 1000) * 1000000
	};
	
	nanosleep(&ts, NULL);
	
    }
    

    printf("\n[Chat] Connecting... Enter messages:\n");
    char buffer[1024];
    
    while (1) {
        printf("[Chat] ");
        fflush(stdout);
        
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strcmp(buffer, "quit") == 0) break;
        
        sendto(sock, buffer, strlen(buffer), 0,
               (struct sockaddr*)&peer, sizeof(peer));
    }
    
    pthread_cancel(listener);
    close(sock);
    return 0;
}
