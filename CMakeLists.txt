cmake_minimum_required(VERSION 3.26)
project(scientific-computing-toolbox)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

set(EIGEN_DIR "include/eigen-3.4.0")
include_directories(${EIGEN_DIR})
set(BOOST_DIR "include/boost-1.83.0")
include_directories(${BOOST_DIR})
set(MU_PARSER_DIR "include/muparser-2.3.4")
include_directories(${MU_PARSER_DIR})

add_subdirectory("include/muparser-2.3.4")

add_executable(exec_target main.cpp)

# Add interpolators library
add_library(interpolators SHARED
        interpolation/interpolator.cpp
        interpolation/cardinal_cubic_bspline_Interpolator.cpp
        interpolation/linear_interpolator.cpp
        interpolation/polynomial_interpolator.cpp
)

# Include directories for interpolators library
target_include_directories(interpolators PUBLIC ${BOOST_DIR})
target_include_directories(interpolators PUBLIC ${EIGEN_DIR})


target_include_directories(exec_target PUBLIC ${MU_PARSER_DIR})


# Link the interpolators library to main executable
target_link_libraries(exec_target PRIVATE interpolators   muparser)