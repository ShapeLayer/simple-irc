#ifndef _SIMPLE_IRC_CLIENT_DESC_H_
#define _SIMPLE_IRC_CLIENT_DESC_H_

struct client_desc {
  int socket;
  char nickname[32];
} typedef client_desc_t;

#endif  // _SIMPLE_IRC_CLIENT_DESC_H_
