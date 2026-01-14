#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#include "client_sess.h"
#include "panic.h"

void init_client_sess(client_sess_t *sess)
{
  sess->sock = -1;
  client_sess_set_nickname(sess, "Guest");

  if ((sess->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    PANIC("Error during socket creation (socket)");

  sess->addr.sin_family = AF_INET;
  client_sess_set_addr(sess, "127.0.0.1", SERVER_PORT);
}

void client_sess_set_nickname(client_sess_t *sess, const char *nickname)
{
  memset(sess->nickname, 0, sizeof(sess->nickname));
  strncpy(sess->nickname, nickname, sizeof(sess->nickname) - 1);
}

void client_sess_set_addr(client_sess_t *sess, const char *addr, int port)
{
  sess->addr.sin_port = htons(port);

  if (inet_pton(AF_INET, addr, &sess->addr.sin_addr) <= 0)
    PANIC("Invalid address. Address not supported (inet_pton)");
}

void client_sess_connect(client_sess_t *sess)
{
  if (connect(sess->sock, (struct sockaddr *)&sess->addr, sizeof(sess->addr)) < 0)
    PANIC("Connection Failed (connect)");

  printf("Connected to server!\n");
  printf("Commands: /nick <name> to change nickname.\n");
}

void client_sess_update(client_sess_t *sess)
{
  FD_ZERO(&sess->readfds);
  FD_SET(STDIN_FILENO, &sess->readfds);
  FD_SET(sess->sock, &sess->readfds);

  if (select(sess->sock + 1, &sess->readfds, NULL, NULL, NULL) < 0)
  {
    PANIC("Error during select operation (select)");
  }

  // handling incoming messages
  if (FD_ISSET(sess->sock, &sess->readfds))
  {
    memset(sess->__str_buf, 0, STRING_BUFFER_SIZE);
    sess->__valread = read(sess->sock, sess->__str_buf, STRING_BUFFER_SIZE);
    
    if (sess->__valread == 0)
    {
      client_sess_on_disconnected(sess);
      return;
    }
    
    printf("%s", sess->__str_buf);
  }

  // handling message input
  if (FD_ISSET(0, &sess->readfds))
  {
    memset(sess->__str_buf, 0, STRING_BUFFER_SIZE);
    if (fgets(sess->__str_buf, STRING_BUFFER_SIZE, stdin) != NULL)
    {
      send(sess->sock, sess->__str_buf, strlen(sess->__str_buf), 0);
    }
  }
}

void client_sess_on_disconnected(client_sess_t *sess)
{
  printf("Disconnected from server.\n");
  exit(EXIT_SUCCESS);
}

void client_sess_disconnect(client_sess_t *sess)
{
  close(sess->sock);
}
