#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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

uint8_t CreateChannelForPeer(const char* peer_ip, const int peer_port, const int my_port) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }
    
    struct sockaddr_in my_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(my_port)
    };
    
    if (bind(sock, (struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) {
        perror("bind");
        close(sock);
        return 1;
    }
    
    struct sockaddr_in peer_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(peer_port)
    };


    if (inet_pton(AF_INET, peer_ip, &peer_addr.sin_addr) != 1) {
      perror("peer ip");
      close(sock);
      return 1;
    }
    
    ThreadArgs args = {sock, peer_addr};
    pthread_t listener;
    pthread_create(&listener, NULL, listen_thread, &args);
    
    printf("[*] Hole punching vers %s:%d...\n", peer_ip, peer_port);
    
    for (int i = 0; i < 100; i++) {
        char msg[32];
        snprintf(msg, sizeof(msg), "PING-%d", i);
        sendto(sock, msg, strlen(msg), 0,
               (struct sockaddr*)&peer_addr, sizeof(peer_addr));
        printf("[*] Envoyé %s\n", msg);
        usleep(100000); 
    }
    

    printf("\n[Chat] Connexion en cours... Tapez vos messages:\n");
    char buffer[1024];
    
    while (1) {
        printf("[Chat] ");
        fflush(stdout);
        
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strcmp(buffer, "quit") == 0) break;
        
        sendto(sock, buffer, strlen(buffer), 0,
               (struct sockaddr*)&peer_addr, sizeof(peer_addr));
    }
    
    pthread_cancel(listener);
    close(sock);
    return 0;
}
