cmake_minimum_required(VERSION 3.15)
project(Algorithms_GraphExtra)

set(CMAKE_CXX_STANDARD 17)

add_library(GraphExtraLib SHARED GraphExtra.cpp ListGraph.cpp MatrixGraph.cpp GraphTraversal.cpp SetGraph.cpp EdgeSubGraph.cpp EdgeSubGraph.h SetSubGraph.cpp SetSubGraph.h)

