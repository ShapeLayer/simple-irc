#include <string.h>
#include <sys/socket.h>

#include "socket.h"

void send_to_socket(int sock, const char *msg)
{
  send(sock, msg, strlen(msg), 0);
}
