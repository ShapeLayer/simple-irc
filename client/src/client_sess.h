#ifndef _CLIENT_SESS_H_
#define _CLIENT_SESS_H_

#include <netinet/in.h>

#include "defines.h"

struct client_sess
{
  int sock;
  struct sockaddr_in addr;
  char nickname[32];
  fd_set readfds;
  
  int __valread;
  char __str_buf[STRING_BUFFER_SIZE];
} typedef client_sess_t;

void init_client_sess(client_sess_t *sess);
void client_sess_set_nickname(client_sess_t *sess, const char *nickname);
void client_sess_set_addr(client_sess_t *sess, const char *addr, int port);
void client_sess_connect(client_sess_t *sess);
void client_sess_update(client_sess_t *sess);
void client_sess_on_disconnected(client_sess_t *sess);
void client_sess_disconnect(client_sess_t *sess);

#endif  // _CLIENT_SESS_H_
