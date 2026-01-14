#ifndef _SIMPLE_IRC_COMMAND_H_
#define _SIMPLE_IRC_COMMAND_H_

#include "server_sess.h"

#define COMMAND_NICK_IS_TRIGGERED(buffer) (strncmp(buffer, "/nick ", 6) == 0)
void command_serv_handle_nick(server_sess_t *sess, int index, char *buffer);

#endif  // _SIMPLE_IRC_COMMAND_H_
