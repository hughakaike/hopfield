cmake_minimum_required(VERSION 3.1)
project(hopfield C)
add_executable(main main.c matrix.c hopfield.c) 
target_link_libraries(main m) 