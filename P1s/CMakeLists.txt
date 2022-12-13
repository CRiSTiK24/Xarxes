cmake_minimum_required(VERSION 3.23)
project(server C)

set(CMAKE_C_STANDARD 99)

add_executable(server UEBp1-aUEBs.c UEBp1-ser.c UEBp1-tTCP.c)
