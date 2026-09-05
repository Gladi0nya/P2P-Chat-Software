/**
 * @file node.c
 * @brief Node module
 *
 * @author Tom Schmitt
 *
 * Copyright (c) 2026 Tom Schmitt
 * All rights reserved.
 *
 */

#include "node.h"

#include "dispatch/dispatcher.h"

#include "net/hole_punch.h"
#include "net/ping.h"

#include "ui/chat_input.h"
#include "protocol/message.h"
#include "logger/logger.h"

#include <poll.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define PUNCH_INTERVAL_MS   500
#define  PING_INTERVAL_MS 10000

int node_run(peer_context_t* ctx) {
  struct pollfd fds[2];

  fds[0].fd     = ctx->sock;
  fds[0].events = POLLIN;
  fds[1].fd     = STDIN_FILENO;
  fds[1].events = POLLIN;

  printf("Awaiting peer...");
  fflush(stdout);
  
  while (ctx->state != PEER_DISCONNECTED) {
    int timeout_ms, ret;

    if (ctx->state == PEER_PUNCHING)     timeout_ms = PUNCH_INTERVAL_MS; 
    else if (ctx->state == PEER_ETABLISHED) timeout_ms = PING_INTERVAL_MS;
    else timeout_ms = -1;
      
    ret = poll(fds, 2, timeout_ms);

    if (ret < 0) {
      LOG_ERROR("poll() failed.");
      break;
    }

    if (ret == 0 && ctx->state == PEER_PUNCHING)
      hole_punch_send_one(ctx);

    if (ret == 0 && ctx->state == PEER_ETABLISHED && (ctx->flag & 0x1))
      send_ping(ctx);
    
    // Net events
    if (fds[0].revents & POLLIN) {
      uint8_t buffer[1024];

      struct sockaddr_in from_addr;
      socklen_t addr_len = sizeof(from_addr);

      ssize_t n = recvfrom(ctx->sock, buffer, sizeof(buffer) - 1, 0,
			   (struct sockaddr*)&from_addr, &addr_len);

      if (n < 0)
	LOG_ERROR("recvfrom() error.");
      
      else if (n >= (ssize_t)sizeof(opcode_t)) {

	buffer[n] = '\0';

	opcode_t op = *(opcode_t*)buffer;

	dispatch_message(ctx, op, buffer + sizeof(opcode_t), (int)(n - sizeof(opcode_t)));

      } else
	LOG_WARNING("Unknown message received.");

      continue;
    }

    // Keyboard events
    if (fds[1].revents & POLLIN) {
      chat_input_handle(ctx);
    }
  }

  LOG_INFO("Node loop exited.");

  return 0;
}
