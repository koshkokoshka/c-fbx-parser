cmake_minimum_required(VERSION 3.0)
project(fbx_parser C)

set(CMAKE_C_STANDARD 90)

add_executable(fbx_parser main.c fbx.c)
