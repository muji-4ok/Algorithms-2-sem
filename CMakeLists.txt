cmake_minimum_required(VERSION 3.15)
project(BigInt)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_library(BigInteger BigInteger.cpp)
add_library(Rational Rational.cpp)
target_link_libraries(Rational BigInteger)

add_executable(BigIntMain main.cpp)
target_link_libraries(BigIntMain BigInteger Rational)

add_executable(CTest mainTests.cpp BigInteger_test.cpp)
target_include_directories(CTest PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(CTest ${GTEST_LIBRARIES} Threads::Threads)
target_link_libraries(CTest Rational BigInteger)
