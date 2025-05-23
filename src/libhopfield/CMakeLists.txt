add_library(hopfield hopfield.c)
target_include_directories(hopfield PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(hopfield PRIVATE matrix)