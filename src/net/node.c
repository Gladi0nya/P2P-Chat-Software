/**
 * @file node.c
 * @brief Client node
 *
 * @author Tom Schmitt
 *
 * Copyright (c) 2026 Tom Schmitt
 * All rights reserved.
 *
 */

#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include "node.h"
#include "protocol.h"
#include "net_helper.h"

#include "random.h"

#include "logger.h"

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

uint8_t is_both_connected = 0;
uint8_t is_closed        = 1;

void* listen_thread(void* arg) {
  ThreadArgs* args = (ThreadArgs*)arg;
  char buffer[1024], packet[1024];
  struct sockaddr_in from_addr;
  socklen_t addr_len = sizeof(from_addr);
  uint64_t id;
  
    while (1) {
    memset(buffer, 0, sizeof(buffer));
    const int n = recvfrom(args->sock, buffer, sizeof(buffer) - 1, 0,
			   (struct sockaddr*)&from_addr, &addr_len);
    
    if (n > 0) {
      const opcode_t op = *(opcode_t*)buffer;
      
      buffer[n] = '\0';
      
      switch (op) {
      case OP_PUNCH:
	if (is_both_connected)
	  continue;
	
	id = *((uint64_t*)(buffer + sizeof(opcode_t)));
        #ifdef __x86_64__
	LOG_DEBUG("punch ID: %lu", id);
        #else
        LOG_DEBUG("punch ID: %llu", id);
        #endif

	*(opcode_t*)packet = OP_PUNCH_ACK;
	*(uint64_t*)(packet + sizeof(opcode_t)) = id;
	
        sendto(args->sock, packet, sizeof(opcode_t) + sizeof(uint64_t),
			      0, (struct sockaddr*)&from_addr, addr_len);
	
	break;	
      case OP_PUNCH_ACK:
	if (is_both_connected)
	  continue;

	is_both_connected = 1;
	is_closed = 0;
	
	id = *((uint64_t*)(buffer + sizeof(opcode_t)));

        #ifdef __x86_64__
        LOG_DEBUG("peer ack ID: %lu", id);
        #else
	LOG_DEBUG("peer ack ID: %llu", id);
        #endif

	*(opcode_t*)packet = OP_PUNCH_ACK;
	*(uint64_t*)(packet + sizeof(opcode_t)) = id;
	
        sendto(args->sock, packet, sizeof(opcode_t) + sizeof(uint64_t),
			      0, (struct sockaddr*)&from_addr, addr_len);
	
	break;
      case OP_MSG:
	if (is_both_connected) {
	  printf("\n[Pair] %s\n[Chat] ", buffer + sizeof(opcode_t));
	}
	break;
      case OP_CLOSE:
	if (is_both_connected) {
	  printf("\nPeer closed the connection.");
	  is_closed = 1;
	}
      }
    }
  }
  
  return NULL;
}

uint8_t node_add_peer(int sock, addr_t peer_addr) {
  struct sockaddr_in peer = {
    .sin_family = AF_INET,
    .sin_port = htons(peer_addr.port),
    .sin_addr.s_addr = peer_addr.ip
  };

  struct sockaddr_in local;
  socklen_t len = sizeof(local);
  getsockname(sock, (struct sockaddr*)&local, &len);

  LOG_INFO("Local port: %u", ntohs(local.sin_port));  
  LOG_INFO("Hole punching to %u%u%u%u:%u...", *(uint8_t*)&peer_addr.ip, *(uint8_t*)(&peer_addr.ip + 1), *(uint8_t*)(&peer_addr.ip + 2), *(uint8_t*)(&peer_addr.ip + 3), peer_addr.port);
  
  ThreadArgs* args = malloc(sizeof(ThreadArgs));
  if (!args) {
    LOG_ERROR("malloc failed");
    return 1;
  }

  args->sock = sock;
  args->peer_addr = peer;
    
  pthread_t listener;
  if (pthread_create(&listener, NULL, listen_thread, args) != 0) {
    LOG_ERROR("pthread_create() failed");

    free(args);
    return 1;
  }

  pthread_detach(listener);
  
  nat_punch_hole_until_cond(sock, OP_PUNCH, (struct sockaddr*)&peer, &is_both_connected);

  char buffer[1024];

  LOG_INFO("P2P Ready, both sides connected.");
    
  while (!is_closed) {
    printf("[Chat] ");
    fflush(stdout);

    *(opcode_t*)buffer = OP_MSG;
    
    if (fgets(buffer + sizeof(opcode_t), sizeof(buffer) - sizeof(opcode_t), stdin) == NULL) break;
    buffer[strcspn(buffer + sizeof(opcode_t), "\n")] = '\0';
        
    if (strcmp(buffer + sizeof(opcode_t), "quit") == 0) break;

    printf("%s\n", buffer + sizeof(opcode_t));
    
    sendto(sock, buffer, strlen(buffer + sizeof(opcode_t)) + sizeof(opcode_t), 0,
	   (struct sockaddr*)&peer, sizeof(peer));
  }

  *(opcode_t*)buffer = OP_CLOSE;
  
  sendto(sock, buffer, sizeof(opcode_t), 0,
	 (struct sockaddr*)&peer, sizeof(peer));
    
  #if defined(__ANDROID__)
  pthread_kill(listener, 0);
  #else
  pthread_cancel(listener);
  #endif

  //  free(ThreadArgs); 
  
  return 0;
}
