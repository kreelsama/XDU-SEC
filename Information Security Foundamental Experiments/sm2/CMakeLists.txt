cmake_minimum_required(VERSION 3.6)
project(sm2 C)

set(CMAKE_C_STANDARD 11)
INCLUDE_DIRECTORIES(../miracl)
LINK_DIRECTORIES(../)

add_executable(sm2 main.c sm3.c sm2.h)
target_link_libraries(sm2 miracl_win)