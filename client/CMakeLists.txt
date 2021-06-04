cmake_minimum_required(VERSION 3.17)
project(client C)

set(CMAKE_C_STANDARD 11)

add_executable(client main.c cJSON.c)

target_link_libraries(client ws2_32)