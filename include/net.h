#ifndef NET_H
#define NET_H

#include <stdint.h>

#include "net_type.h"

int net_init(uint16_t chosen_port);
int net_create_chat(char* peer_ip, char* peer_port);
int net_shutdown(void);

#endif
