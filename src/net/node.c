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

uint8_t isConnected = 0;
uint8_t isPeerConnected = 0;

void* listen_thread(void* arg) {
  ThreadArgs* args = (ThreadArgs*)arg;
  char buffer[1024];
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
	if (isConnected)
	  continue;

	isConnected = 1;
	
	id = *((uint64_t*)(buffer + sizeof(opcode_t)));
        #ifdef __x86_64__
	LOG_DEBUG("punch ID: %lu\n", id);
        #else
        LOG_DEBUG("punch ID: %llu\n", id);
        #endif
	
	break;
      case OP_ACK:
	if (isPeerConnected)
	  continue;
	isConnected = 1;
	isPeerConnected = 1;
	
	id = *((uint64_t*)(buffer + sizeof(opcode_t)));

        #ifdef __x86_64__
        LOG_DEBUG("peer ack ID: %lu\n", id);
        #else
	LOG_DEBUG("peer ack ID: %llu\n", id);
        #endif
	
	break;
      case OP_MSG:
	if (isPeerConnected && isConnected) {
	  printf("\n[Pair] %s\n", buffer + sizeof(opcode_t));
	  fflush(stdout);
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

  LOG_INFO("Local port: %u\n", ntohs(local.sin_port));  
  LOG_INFO("Hole punching to %u%u%u%u:%u...\n", *(uint8_t*)&peer_addr.ip, *(uint8_t*)(&peer_addr.ip + 1), *(uint8_t*)(&peer_addr.ip + 2), *(uint8_t*)(&peer_addr.ip + 3), peer_addr.port);
  
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
  
  nat_punch_hole_until_cond(sock, OP_PUNCH, (struct sockaddr*)&peer, &isConnected);

  nat_punch_hole_until_cond(sock, OP_ACK, (struct sockaddr*)&peer, &isPeerConnected);

  char buffer[1024];

  LOG_INFO("P2P Ready, both sides connected.");
    
  while (1) {
    printf("[Chat] ");
    fflush(stdout);

    *(opcode_t*)buffer = OP_MSG;
    
    if (fgets(buffer + sizeof(opcode_t), sizeof(buffer) - sizeof(opcode_t), stdin) == NULL) break;
    buffer[strcspn(buffer + sizeof(opcode_t), "\n")] = '\0';
        
    if (strcmp(buffer + sizeof(opcode_t), "quit") == 0) break;
        
    sendto(sock, buffer, strlen(buffer), 0,
	   (struct sockaddr*)&peer, sizeof(peer));
  }
    
  #if defined(__ANDROID__)
  pthread_kill(listener, 0);
  #else
  pthread_cancel(listener);
  #endif

  //  free(ThreadArgs);
  
  return 0;
}
