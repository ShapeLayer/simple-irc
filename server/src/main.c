#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#include "defines.h"
#include "client.h"
#include "panic.h"

#include "server_sess.h"

struct main_args
{
  int port;
} typedef main_args_t;

void print_usage()
{
  printf("Simple IRC Server\n");
  printf("Usage: ./simple-irc-server\n");
  printf("  -p, --port <port>    Specify the port to run the server on (default: %d)\n", SERVER_PORT);
}

main_args_t parse_args(int argc, char const **argv)
{
  main_args_t args;
  args.port = SERVER_PORT;
  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0)
    {
      if (i + 1 < argc)
      {
        int port = atoi(argv[i + 1]);
        if (port <= 0 || port > 65535)
        {
          printf("Invalid port number: %s\n", argv[i + 1]);
          print_usage();
          PANIC("Invalid port number");
        }
        args.port = port;
        i++;  // skip next argument as it's the port number
      }
      else
      {
        printf("Port number not specified after %s\n", argv[i]);
        print_usage();
        exit(EXIT_FAILURE);
      }
    }
    else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
    {
      print_usage();
      exit(EXIT_SUCCESS);
    }
    else
    {
      printf("Unknown argument: %s\n", argv[i]);
      print_usage();
      exit(EXIT_FAILURE);
    }
  }
  return args;
}

int main(int argc, char const **argv)
{
  main_args_t args = parse_args(argc, argv);

  server_sess_t *server_sess;
  server_sess = calloc(1, sizeof(server_sess_t));
  if (!server_sess) PANIC("Failed to allocate server session");
  init_server_sess(server_sess);
  printf("Server session initialized.\n");
  server_sess_set_port(server_sess, args.port);
  printf("Starting server on port %d...\n", args.port);
  server_sess_open(server_sess);
  while (1) server_sess_update(server_sess);
  return 0;
}
