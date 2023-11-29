cmake_minimum_required(VERSION 3.26)
project(scientific-computing-toolbox)

set(CMAKE_CXX_STANDARD 17)

#add_compile_options(-Wall -Wextra -Wpedantic -Werror)

set(EIGEN_DIR "../eigen-3.3.9")
include_directories(${EIGEN_DIR})
set(BOOST_DIR "../boost_1_83_0")
include_directories(${BOOST_DIR})
set(GSL_DIR "../gsl-2.7.1")
include_directories(${GSL_DIR})
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

# Link the interpolators library to main executable
target_link_libraries(exec_target PRIVATE interpolators)