cmake_minimum_required(VERSION 3.26)
project(scientific-computing-toolbox)

set(CMAKE_CXX_STANDARD 17)

#add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Eigen
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# GSL
find_package(GSL REQUIRED)

# Boost
find_package(Boost REQUIRED COMPONENTS math_c99l)

# Statistics library
add_library(statistics SHARED
        statistics/statistics.cpp
)

# Link libraries to the 'statistics' library
target_link_libraries(statistics PUBLIC
        Eigen3::Eigen
        GSL::gsl GSL::gslcblas
)

# Include directory for the statistics library
target_include_directories(statistics PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/statistics
)

# Interpolation library
add_library(interpolation SHARED
        interpolation/cardinal_cubic_bspline_Interpolator.cpp
        interpolation/linear_interpolator.cpp
        interpolation/polynomial_interpolator.cpp
)

# Link libraries to the interpolation library
target_link_libraries(interpolation PUBLIC
        Eigen3::Eigen
        Boost::math_c99l
)

# Include directory for the interpolation library
target_include_directories(interpolation PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/interpolation
)

add_executable(toolbox main.cpp)
target_link_libraries(toolbox PRIVATE statistics interpolation)
