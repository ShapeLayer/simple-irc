#include <stdio.h>
#include <string.h>

#include "command.h"
#include "defines.h"
#include "server_sess.h"
#include "socket.h"

void command_serv_handle_nick(server_sess_t *sess, int index, char *buffer)
{
  char *new_nick = buffer + 6;
  char noti[STRING_BUFFER_SIZE];

  // notify
  snprintf(noti, sizeof(noti), "[Server] %s changed name to %s\n", sess->clients[index].nickname, new_nick);
  SERVER_SESS_LOG_AND_BRODCAST(sess, index, noti);
  
  // update nickname
  strncpy(sess->clients[index].nickname, new_nick, 31);

  char confirm[STRING_BUFFER_SIZE];
  snprintf(confirm, sizeof(confirm), "[Server] Nickname changed to %s\n", sess->clients[index].nickname);
  send_to_socket(sess->clients[index].socket, confirm);
}
