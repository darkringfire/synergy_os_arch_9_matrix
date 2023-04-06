cmake_minimum_required(VERSION 3.18)
project(synergy_os_arch_9_matrix)

set(CMAKE_CXX_STANDARD 17)

add_executable(pipes main_pipes.cpp)
add_executable(matrix_mul main.cpp)
