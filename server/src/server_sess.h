#ifndef _SIMPLE_IRC_SERVER_SESS_H_
#define _SIMPLE_IRC_SERVER_SESS_H_

#include <netinet/in.h>
#include <sys/select.h>

#include "client.h"
#include "defines.h"

struct server_sess
{
  int fd;
  int opt;
  int sd_cnt;
  int port;
  struct sockaddr_in address;
  int addrlen;
  client_desc_t clients[SERVER_MAX_CLIENT_CONNECTIONS];
  fd_set readfds;

  /**
   * 매 루프에서 임시로 사용되는 변수:
   * server_update에서 매 회 생성할 수도 있으나, while 루프 안에서
   * 매 회 생성하는 데 코스트를 넣는 것보다는 서버 세션 기술자에 포함시켜
   * 1회만 생성하는 것이 낫다고 판단함.
   */
  int __new_socket;
  int __valread;
  int __activity;
  char __str_buf[STRING_BUFFER_SIZE];
} typedef server_sess_t;

void init_server_sess(server_sess_t *sess);
void server_sess_update(server_sess_t *sess);
void server_sess_open(server_sess_t *sess);
void server_sess_set_port(server_sess_t *sess, int port);
void server_sess_broadcast(server_sess_t *sess, int sender_idx, const char *msg);
void server_sess_handle_client_message(server_sess_t *sess, int index, char *buffer);

#define SERVER_SESS_LOG_AND_BRODCAST(sess, idx, msg) { \
  printf("%s", msg); /* log to server */ \
  server_sess_broadcast(sess, idx, msg); /* broadcast to all */ \
}

#endif  // _SIMPLE_IRC_SERVER_SESS_H_
