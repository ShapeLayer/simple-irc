cmake_minimum_required(VERSION 3.20)
project(simple_irc_client C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(HEADERS
  src/client_sess.h
  include/client.h
  include/defines.h
  include/panic.h
)

set(SOURCES
  src/client_sess.c
  src/main.c
)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
  include
)
