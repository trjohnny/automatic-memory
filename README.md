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

## California Housing Prices dataset analysis

To test the statistics library, we decided to perform a statistical analysis of the famous California Housing Prices 
dataset as follows.

### Numerical Data Statistics
#### Location (Longitude and Latitude):

The mean and median values for longitude and latitude suggest the geographic center of the dataset. The standard 
deviation indicates the spread, which is relatively moderate, suggesting that the dataset covers a specific geographic 
region without extreme variation.

#### Housing Median Age:

The mean and median are close, indicating a relatively symmetric age distribution of houses. The standard deviation and 
variance are relatively high, implying a diverse range of house ages.

#### Total Rooms, Bedrooms, Population, and Households:

These attributes have high means and standard deviations, indicating a wide variance in the sizes of houses and 
populations across the dataset. The significantly higher standard deviation for total rooms shows a greater diversity in house sizes.

#### Median Income and House Value:

These economic indicators show a wide range of values, as evidenced by their high standard deviations and variances. 
The difference between the mean and median in house values suggests a right-skewed distribution, possibly due to some 
very high-value houses.

### Categorical Data Frequency Counts

#### Ocean Proximity:
This category varies significantly, with 'INLAND' and '<1H OCEAN' being the most frequent categories. The presence of 
only 5 'ISLAND' instances indicates that it's a rare category in this dataset.

### Correlation Matrix
The correlation matrix shows the relationships between different numerical variables. High correlation values (close 
to 1) indicate a strong linear relationship.

#### Housing Median Age and Location Coordinates (Longitude: 0.961, Latitude: 0.946):

The very strong positive correlations between housing median age and both longitude and latitude are intriguing. 
This suggests a distinct geographical trend where certain areas, defined by their coordinates, predominantly consist of 
older houses. This pattern might reflect historical development trends in the region, where older neighborhoods are 
concentrated in specific parts of the geographical landscape.

#### Total Rooms and Population (0.992), and Total Rooms and Households (0.998):

The near-perfect correlation between total rooms and both population and households is notable. While it is somewhat 
expected that larger homes (with more rooms) would house more people, the strength of this correlation is striking. 
It indicates a very consistent pattern across the dataset where the number of rooms in a house almost directly predicts 
he size of the household and the associated population. This consistency might suggest standardized housing development 
practices or a uniformity in family sizes relative to house sizes in this region.

#### Median Income and Median House Value (0.962):

The strong positive correlation between median income and median house value, while somewhat expected, is particularly 
interesting in its strength. This correlation is a clear indicator of the economic principle that areas with higher 
incomes can sustain higher housing prices. However, the strength of this correlation (nearly 1) might also suggest 
limited economic diversity within neighborhoods or a strong stratification where higher-income individuals are 
concentrated in more expensive areas, possibly leading to economic segregation.