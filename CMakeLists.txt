cmake_minimum_required(VERSION 3.15)
project(ht10)

set(CMAKE_CXX_STANDARD 14)

add_executable(ht10 main.cpp tcp_client.c tcp_server.c)