cmake_minimum_required(VERSION 3.15)
project(Algorithms_2sem)

set(CMAKE_CXX_STANDARD 17)

add_executable(GraphExtra GraphExtra/Graph.h GraphExtra/GraphExtra.cpp GraphExtra/ListGraph.cpp GraphExtra/ListGraph.h GraphExtra/MatrixGraph.cpp GraphExtra/MatrixGraph.h GraphExtra/GraphTraversal.cpp GraphExtra/GraphTraversal.h)