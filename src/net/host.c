/**
 * @file host.c
 * @brief Host conversion utilities
 *
 * @author Tom Schmitt
 *
 * Copyright (c) 2026 Tom Schmitt
 * All rights reserved.
 *
 */

#include "host.h"

#include <string.h>

#include <netdb.h>


/** ----------------------------------------------------------- *
  *  resolve_domain                                             *
  *                                                             *
  *  Convert domain name server into public IP.                 *
  *                                                             *
  *  @param hostname  [in] Domain name server.                  *
  *  @param out_addr [out] Public IP address.                   *
  *                                                             *
  *  @retval 0 Host conversion successful.                      *
  *  @retval 1 Host comversion failed.                          *
  * ----------------------------------------------------------- **/

int resolve_domain(const char* const restrict hostname, struct sockaddr_in* out_addr) {
  struct addrinfo hints = {0};
  struct addrinfo* result;
  
  hints.ai_family   = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  if (getaddrinfo(hostname, NULL, &hints, &result) != 0) return 1;

  memcpy(out_addr, result->ai_addr, sizeof(struct sockaddr_in));
  
  freeaddrinfo(result);
  
  return 0;
}
