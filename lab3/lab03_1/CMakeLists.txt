cmake_minimum_required(VERSION 3.10)

project(lab03_1 C)

file(GLOB C_SOURCES "*.c")

add_executable(lab3_1 ${C_SOURCES})