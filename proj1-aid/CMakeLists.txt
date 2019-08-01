cmake_minimum_required(VERSION 3.6)
project(proj1_aid)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES
    flight_map.c
    flight_map.h
    tests.c)

add_executable(proj1_aid ${SOURCE_FILES})