cmake_minimum_required(VERSION 3.26)
project(scientific-computing-toolbox)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wpedantic -Werror -Wextra)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(EIGEN_DIR "include/eigen-3.4.0")
set(BOOST_DIR "include/boost-1.83.0")
set(STATISTICS_DIR "statistics")
set(INTERPOLATORS_DIR "interpolation")
include_directories(${EIGEN_DIR} ${BOOST_DIR} ${STATISTICS_DIR} ${INTERPOLATORS_DIR})

add_executable(scientific-computing-toolbox main.cpp)

# Find or download pybind11
find_package(pybind11 REQUIRED)

# Add Python module
pybind11_add_module(statistics_py MODULE bindings/statistics_python_bindings.cpp)
target_link_libraries(statistics_py PRIVATE statistics)

pybind11_add_module(interpolator_py MODULE bindings/interpolator_python_bindings.cpp)
target_link_libraries(interpolator_py PRIVATE interpolators)

# Add interpolators library
add_library(interpolators SHARED
        interpolation/interpolator.cpp
        interpolation/cardinal_cubic_bspline_Interpolator.cpp
        interpolation/linear_interpolator.cpp
        interpolation/polynomial_interpolator.cpp
)

# Add interpolators library
add_library(statistics SHARED
        statistics/stat_utils.cpp
        statistics/dataset.cpp
)

# Include directories for interpolators library
target_include_directories(interpolators PUBLIC ${BOOST_DIR} ${EIGEN_DIR})
target_include_directories(statistics PUBLIC ${EIGEN_DIR})


# Link the interpolators library to main executable
target_link_libraries(scientific-computing-toolbox PRIVATE interpolators statistics)