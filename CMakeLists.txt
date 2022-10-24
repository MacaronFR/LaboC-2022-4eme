cmake_minimum_required(VERSION 3.24)
project(socket C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_executable(socket src/main.c src/simple.c src/client.c src/complexe.c src/shared.c)
