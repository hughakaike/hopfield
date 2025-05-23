cmake_minimum_required(VERSION 3.1)
project(hopfieldproj C)
set(CMAKE_C_STANDARD 99)

add_subdirectory(libmatrix)
add_subdirectory(libhopfield)

add_executable(main main.c)

target_link_libraries(main PRIVATE matrix hopfield)
target_link_libraries(main PRIVATE m)