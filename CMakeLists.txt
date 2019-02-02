cmake_minimum_required(VERSION 3.13)
project(algo)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-sanitize-recover=all -fsanitize-undefined-trap-on-error -g -std=c++17 -O2 -Wall -Werror")

add_executable(algo main.cpp)
add_executable(data_gen data_gen.cpp)