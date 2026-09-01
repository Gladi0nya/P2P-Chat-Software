#include "chat_input.h"

#include "protocol/message.h"

#include "logger/logger.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

void chat_input_handle(peer_context_t *ctx)
{
  uint8_t buffer[1024];

  if (fgets((char*)(buffer + sizeof(opcode_t)), sizeof(buffer) - sizeof(opcode_t), stdin) == NULL)
  {
    ctx->state = PEER_DISCONNECTED;
    return;
  }

  char *msg = (char*)buffer + sizeof(opcode_t);
  msg[strcspn(msg, "\n")] = '\0';

  if (strncmp(msg, "quit", 4) == 0) {
    *(opcode_t*)buffer = OP_CLOSE;
    sendto(ctx->sock, buffer, sizeof(opcode_t), 0,
	   (struct sockaddr*)&ctx->peer_addr, sizeof(ctx->peer_addr));
    ctx->state = PEER_DISCONNECTED;
    return;
  }

  if (ctx->state != PEER_ETABLISHED) {
    LOG_WARNING("Not connected yet, message dropped.");
    return;
  }

  *(opcode_t*)buffer = OP_MSG;
  if (!sendto(ctx->sock, buffer, strlen(msg) + sizeof(opcode_t), 0,
	      (struct sockaddr*)&ctx->peer_addr, sizeof(ctx->peer_addr)))
    LOG_WARNING("Failed to send message.");

  printf("[CHAT] ");
  fflush(stdout);
}
