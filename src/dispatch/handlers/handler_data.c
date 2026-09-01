#include "handler_data.h"
#include "protocol/message.h"

#include <stdio.h>

void handle_msg(peer_context_t *ctx, uint8_t *buffer, int n) {
  buffer[n] = '\0';
  
  if (ctx->state == PEER_ETABLISHED) {
    printf("\n[PAIR] %s\n[CHAT] ", buffer);
    fflush(stdout);
  }
}
