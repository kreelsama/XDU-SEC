cmake_minimum_required(VERSION 3.12)
project(fermat C)

set(CMAKE_C_STANDARD 11)
set(LINK_DIR ../)
include_directories(../miracl)

LINK_DIRECTORIES(${LINK_DIR})

add_executable(fermat main.c)
target_link_libraries(fermat miracl)