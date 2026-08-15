#include "node.h"

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void holePunch(int sock, struct sockaddr_in* peer_addr)
{
  // Bypass NAT with Hole Punching
  for (int i = 0; i < 10; i++) {
    char msg[16];

    snprintf(msg, sizeof(msg), "PING-%d", i);
    sendto(sock, msg, strlen(msg), 0, (struct sockaddr*)peer_addr, sizeof(struct sockaddr*));

    usleep(100000);
  }
}

uint8_t CreateChannelForPeer(const char* peer_ip, const int peer_port, const int my_port)
{
  int sock;
  struct sockaddr_in my_addr, peer_addr;

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    return 1;

  memset(&my_addr, 0, sizeof(my_addr));

  my_addr.sin_family = AF_INET;
  my_addr.sin_port   = htons(my_port);
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1) {
    close(sock);
    return 1;
  }

  memset(&peer_addr, 0, sizeof(peer_addr));

  peer_addr.sin_family = AF_INET;
  peer_addr.sin_port = htons(peer_port);
  inet_pton(AF_INET, peer_ip, &peer_addr.sin_addr);

  holePunch(sock, &peer_addr);

  char buffer[1024];
  struct sockaddr_in from_addr;
  socklen_t addr_len = sizeof(from_addr);

  struct timeval tv = {30, 0};
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

  int n = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&from_addr, &addr_len);

  if (n > 0) {
    buffer[n] = '\n';
    printf("Connexion etablie.\n");
    printf("[Chat] Tapez vos messages (ou 'quit'):\n");

    while(1) {
      fgets(buffer, sizeof(buffer), stdin);
      buffer[strcspn(buffer, "\n")] = 0;
            
      if (strcmp(buffer, "quit") == 0) break;
            
      sendto(sock, buffer, strlen(buffer), 0,
                   (struct sockaddr*)&peer_addr, sizeof(peer_addr));
            
      n = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr*)&from_addr, &addr_len);
      if (n > 0) {
	buffer[n] = '\0';
	printf("[%s:%d] %s\n", inet_ntoa(from_addr.sin_addr), ntohs(from_addr.sin_port), buffer);
      }
    }
    
  } else {
    printf("Aucune reponse recue.\n");
  }
  
  close(sock);
  return 0;
}
