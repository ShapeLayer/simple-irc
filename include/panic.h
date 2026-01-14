#ifndef _SIMPLE_IRC_PANIC_H_
#define _SIMPLE_IRC_PANIC_H_

#define PANIC(msg) { \
  perror(msg); \
  exit(EXIT_FAILURE); \
}

#endif
