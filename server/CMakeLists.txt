cmake_minimum_required(VERSION 3.20)
project(simple_irc_server C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(HEADERS
  src/command.h
  src/server_sess.h
  src/socket.h
  include/client.h
  include/defines.h
  include/panic.h
)

set(SOURCES
  src/command.c
  src/server_sess.c
  src/socket.c
  src/main.c
)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
  include
)
