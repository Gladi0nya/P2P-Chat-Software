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

uint8_t isConnected     = 0;
uint8_t isPeerConnected = 0;

void* listen_thread(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    char buffer[1024];
    struct sockaddr_in from_addr;
    socklen_t addr_len = sizeof(from_addr);
    
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = recvfrom(args->sock, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr*)&from_addr, &addr_len);
	printf("test %d\n", n);
	fflush(stdout);
	
        if (n > 0) {
	  if (!isConnected)
	    isConnected = 1;

	  if (strcmp(buffer, "connected") == 0)
	    isPeerConnected = 1;
	  
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
    
    char ip_str[INET_ADDRSTRLEN];
    struct in_addr addr = { peer_addr.ip };
    inet_ntop(AF_INET, &addr, ip_str, sizeof(ip_str));
    printf("[*] Hole punching to %s:%u...\n", ip_str, peer_addr.port);
    
    ThreadArgs* args = malloc(sizeof(ThreadArgs));
    if (!args) {
        LOG_ERROR("malloc failed");
        return 1;
    }
    args->sock = sock;
    args->peer_addr = peer;
    
    pthread_t listener;
    if (pthread_create(&listener, NULL, listen_thread, args) != 0) {
        LOG_ERROR("pthread_create failed");
        free(args);
        return 1;
    }
    pthread_detach(listener);

    struct sockaddr_in local;
    socklen_t len = sizeof(local);
    getsockname(sock, (struct sockaddr*)&local, &len);
    printf("[*] Local port: %d\n", ntohs(local.sin_port));
    printf("[*] Target: %s:%d\n", ip_str, peer_addr.port);
    
    while(!isConnected) {
        char msg[32];
        snprintf(msg, sizeof(msg), "PING");
        
        ssize_t sent = sendto(sock, msg, strlen(msg), 0,
                              (struct sockaddr*)&peer, sizeof(peer));
        if (sent < 0) {
            LOG_ERROR("sendto failed.");
        } else {
	  //printf("[*] Sent %s\n", msg);
        }
        
        struct timespec ts = {
            .tv_sec = 0,
            .tv_nsec = 100000000  // 100 ms
        };
        nanosleep(&ts, NULL);
    }

    while(!isPeerConnected) {
        char msg[32];
        snprintf(msg, sizeof(msg), "connected");
        
        ssize_t sent = sendto(sock, msg, strlen(msg), 0,
                              (struct sockaddr*)&peer, sizeof(peer));
        if (sent < 0) {
            LOG_ERROR("sendto failed.");
        } else {
	  //printf("[*] Sent %s\n", msg);
        }
        
        struct timespec ts = {
            .tv_sec = 0,
            .tv_nsec = 100000000  // 100 ms
        };
        nanosleep(&ts, NULL);
    }
    
    printf("\n[Chat] Connected... Enter messages:\n");
    char buffer[1024];
    
    while (1) {
        printf("[Chat] ");
        fflush(stdout);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strcmp(buffer, "quit") == 0) break;
        
        sendto(sock, buffer, strlen(buffer), 0,
               (struct sockaddr*)&peer, sizeof(peer));
    }
    
    pthread_cancel(listener);
    close(sock);
    return 0;
}
