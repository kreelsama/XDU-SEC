cmake_minimum_required(VERSION 3.12)
project(sm3_miracl C)


set(LINK_DIR ../)
include_directories(../miracl)

LINK_DIRECTORIES(${LINK_DIR})


set(CMAKE_C_STANDARD 11)

add_executable(sm3_miracl main.c)
target_link_libraries(sm3_miracl miracl_linux)