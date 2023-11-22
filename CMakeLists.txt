cmake_minimum_required(VERSION 3.26)
project(Scientific_computing)

set(CMAKE_CXX_STANDARD 17)

set(EIGEN_DIR "../eigen-3.4.0")
include_directories(${EIGEN_DIR})
set(BOOST_DIR "../boost_1_83_0")
include_directories(${BOOST_DIR})
add_executable(new_target main.cpp)
