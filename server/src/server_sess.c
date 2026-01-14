#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "command.h"
#include "defines.h"
#include "server_sess.h"
#include "socket.h"
#include "panic.h"

void init_server_sess(server_sess_t *sess)
{
  for (int i = 0; i < SERVER_MAX_CLIENT_CONNECTIONS; i++)
  {
    sess->clients[i].socket = 0;
    snprintf(sess->clients[i].nickname, sizeof(sess->clients[i].nickname), "Guest%d", i);
  }

  if ((sess->fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    PANIC("Error during opening the socket (socket)");
  }

  sess->opt = 1;
  if (setsockopt(sess->fd, SOL_SOCKET, SO_REUSEADDR, &sess->opt, sizeof(sess->opt)))
  {
    PANIC("Error during setting the socket's option (setsockopt)");
  }

  sess->address.sin_family = AF_INET;
  sess->address.sin_addr.s_addr = INADDR_ANY;
  server_sess_set_port(sess, SERVER_PORT);
}

void server_sess_open(server_sess_t *sess)
{
  if (bind(sess->fd, (struct sockaddr *)&sess->address, sizeof(sess->address)) < 0)
  {
    PANIC("Error during binding the socket (bind)");
  }
  sess->addrlen = sizeof(sess->address);

  if (listen(sess->fd, 3) < 0)
  {
    PANIC("Error during starting listening on the socket (listen)");
  }

  printf("Simple Chat Server started on port %d\n", sess->port);
}

void server_sess_set_port(server_sess_t *sess, int port)
{
  sess->address.sin_port = htons(port);
  sess->port = port;
}

void server_sess_update(server_sess_t *sess)
{
  FD_ZERO(&sess->readfds);
  FD_SET(sess->fd, &sess->readfds);
  sess->sd_cnt = sess->fd;

  for (int i = 0; i < SERVER_MAX_CLIENT_CONNECTIONS; i++)
  {
    int sd = sess->clients[i].socket;
    if (sd > 0)
      FD_SET(sd, &sess->readfds);
    if (sd > sess->sd_cnt)
      sess->sd_cnt = sd;
  }

  sess->__activity = select(sess->sd_cnt + 1, &sess->readfds, NULL, NULL, NULL);
  if (sess->__activity < 0) PANIC("Error during select operation (select)");


  // handling new connections
  if (FD_ISSET(sess->fd, &sess->readfds))
  {
    if ((sess->__new_socket = accept(sess->fd, (struct sockaddr *)&sess->address, (socklen_t *)&sess->addrlen)) < 0)
    {
      PANIC("Error during accepting a new connection (accept)");
    }

    for (int i = 0; i < SERVER_MAX_CLIENT_CONNECTIONS; i++)
    {
      if (sess->clients[i].socket == 0)
      {
        sess->clients[i].socket = sess->__new_socket;
        printf("New connection: Guest%d\n", i);

        char welcome[STRING_BUFFER_SIZE];
        snprintf(welcome, sizeof(welcome), "Welcome! You are Guest%d. Use '/nick <name>' to change nickname.\n", i);
        send_to_socket(sess->__new_socket, welcome);
        break;
      }
    }
  }

  // handing receving messages from clients
  for (int i = 0; i < SERVER_MAX_CLIENT_CONNECTIONS; i++)
  {
    int sd = sess->clients[i].socket;
    if (FD_ISSET(sd, &sess->readfds))
    {
      memset(sess->__str_buf, 0, STRING_BUFFER_SIZE);
      if ((sess->__valread = read(sd, sess->__str_buf, STRING_BUFFER_SIZE)) == 0)
      {
        // disconnection
        close(sd);
        sess->clients[i].socket = 0;
        printf("User disconnected: %s\n", sess->clients[i].nickname);
      }
      else
      {
        if (sess->__valread >= STRING_BUFFER_SIZE)
          sess->__str_buf[STRING_BUFFER_SIZE - 1] = '\0';
        else
          sess->__str_buf[sess->__valread] = '\0';
        server_sess_handle_client_message(sess, i, sess->__str_buf);
      }
    }
  }
}

void server_sess_broadcast(server_sess_t *sess, int sender_idx, const char *msg)
{
  for (int i = 0; i < SERVER_MAX_CLIENT_CONNECTIONS; i++)
  {
    if (sess->clients[i].socket != 0 && i != sender_idx)
    {
      send_to_socket(sess->clients[i].socket, msg);
    }
  }
}


void server_sess_handle_client_message(server_sess_t *sess, int index, char *buffer)
{
  buffer[strcspn(buffer, "\r\n")] = 0;

  if (strlen(buffer) == 0) return;

  // command services:
  if (COMMAND_NICK_IS_TRIGGERED(buffer))
  {
    command_serv_handle_nick(sess, index, buffer);
    return;
  }
  
  char msg_out[STRING_BUFFER_SIZE + 50];
  snprintf(msg_out, sizeof(msg_out), "[%s]: %s\n", sess->clients[index].nickname, buffer);

  SERVER_SESS_LOG_AND_BRODCAST(sess, index, msg_out);
}
