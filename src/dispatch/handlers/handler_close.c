#include "handler_close.h"

#include <stdio.h>

void handle_close(peer_context_t *ctx)
{
  if (ctx->state == PEER_ETABLISHED) {
    printf("\nPeer closed the connection.\n");
    ctx->state = PEER_DISCONNECTED;
    fflush(stdout);
  }
}
