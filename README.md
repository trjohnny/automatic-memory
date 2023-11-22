# Statistics Library
## Overview
Statistics Library is a C++ library designed for scientific computing and data analysis. It offers functionalities for statistical analysis, including computations of mean, median, standard deviation, variance, and more.

## Prerequisites
Before installing the Statistics Library, ensure you have the following installed on your system:

CMake (version 3.14 or higher)
A C++ compiler supporting C++17 (e.g., GCC, Clang)
Eigen (version 3.3 or higher)
GNU Scientific Library (GSL)
Git (for fetching dependencies)

## Installation
### Dependencies
The library depends on Eigen, GSL, and csv-parser. Eigen and GSL need to be installed on your system. csv-parser is included as a part of the build process via CMake's FetchContent.

Installing Eigen and GSL
* On Ubuntu/Debian:

  `sudo apt-get install libeigen3-dev libgsl-dev`
* On macOS (using Homebrew):

  `brew install eigen gsl`

### Building the Library

1. Clone the repository:

    `git clone https://github.com/yourusername/StatisticsLibrary.git
    cd StatisticsLibrary`

2. Create a build directory and navigate into it:

    `mkdir build
    cd build`

3. Run CMake and build the project:

    `cmake ..
    make`

## Usage
Include the Statistics Library in your C++ project and link against it. Here's a basic example of using the library:

```
#include "statistics.hpp"

int main() {
// Example usage of the Statistics Library
// ...
return 0;
}
```

## Features
* Statistical analysis tools including mean, median, standard deviation, and variance calculations.
* CSV data parsing and processing capabilities.
* Integration with Eigen for matrix and vector operations.
* GSL-based advanced mathematical computations.