#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defines.h"
#include "client_sess.h"
#include "panic.h"

int main(int argc, char const **argv)
{
  client_sess_t *client_sess;
  client_sess = calloc(1, sizeof(client_sess_t));
  if (!client_sess) PANIC("Failed to allocate client session");

  init_client_sess(client_sess);
  client_sess_connect(client_sess);

  while (1) client_sess_update(client_sess);

  client_sess_disconnect(client_sess);
  return 0;
}
