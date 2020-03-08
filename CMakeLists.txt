cmake_minimum_required(VERSION 3.15)
project(Algorithms_2sem)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(GoogleTests)
add_subdirectory(GraphExtra)
include_directories(GraphExtra)

add_executable(Task_1_D Task_1_d.cpp GraphExtra/EdgeSubGraph.cpp GraphExtra/EdgeSubGraph.h GraphExtra/SetSubGraph.cpp GraphExtra/SetSubGraph.h)
target_link_libraries(Task_1_D GraphExtraLib)
