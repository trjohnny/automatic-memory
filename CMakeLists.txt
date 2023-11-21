cmake_minimum_required(VERSION 3.26)
project(scientific-computing-toolbox)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# CSV Parser
FetchContent_Declare(
        csvparser
        GIT_REPOSITORY https://github.com/vincentlaucsb/csv-parser.git
)

FetchContent_MakeAvailable(csvparser)

FetchContent_GetProperties(csvparser)
set(CSV_PARSER_INCLUDE_DIR ${csvparser_SOURCE_DIR}/single_include)

# Eigen
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# GSL
find_package(GSL REQUIRED)

# Statistics library
add_library(statistics SHARED
        statistics/statistics.cpp
        # Add other source files here
)

# Link libraries to the 'statistics' library
target_link_libraries(statistics PUBLIC
        Eigen3::Eigen
        GSL::gsl GSL::gslcblas
)

# Include directory for the statistics library
target_include_directories(statistics PUBLIC
        ${CSV_PARSER_INCLUDE_DIR}
)

add_executable(toolbox main.cpp)
target_link_libraries(toolbox PRIVATE statistics)