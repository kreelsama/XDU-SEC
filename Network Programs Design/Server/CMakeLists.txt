cmake_minimum_required(VERSION 3.12)
project(proj C)

set(CMAKE_C_STANDARD 11)


add_executable(proj main.c flag.h)
target_link_libraries(proj sqlite3)
target_link_libraries(proj pthread)