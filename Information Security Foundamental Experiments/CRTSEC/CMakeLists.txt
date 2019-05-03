cmake_minimum_required(VERSION 3.12)
project(CRTSEC C)

set(CMAKE_C_STANDARD 11)
INCLUDE_DIRECTORIES(../miracl)
LINK_DIRECTORIES(../)

add_executable(CRTSEC main.c)
target_link_libraries(CRTSEC miracl_linux)