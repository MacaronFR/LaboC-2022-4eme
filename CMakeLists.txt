cmake_minimum_required(VERSION 3.24)
project(socket C)

set(CMAKE_C_STANDARD 99)

add_executable(socket main.c simple.c simple.h client.c complexe.c)
