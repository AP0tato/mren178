cmake_minimum_required(VERSION 3.10)

project(lab3_2 C)

file(GLOB C_SOURCES "*.c")

file(GLOB_RECURSE C_HEADERS "*.h")

add_executable(lab3_2 ${C_SOURCES} ${C_HEADERS})