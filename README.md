# Scientific Computing Toolbox
## Overview
The Scientific Computing Toolbox is a comprehensive C++ library designed for advanced scientific computing and data 
analysis. It includes two key modules as shared libraries: the Statistics Library and the Interpolator Module, each 
offering specialized functionalities for a broad range of applications.

## Prerequisites
Before installing the Scientific Computing Toolbox, ensure you have the following installed on your system:

* CMake (version 3.14 or higher)
* A C++ compiler supporting C++17 (e.g., GCC, Clang)
* Eigen (version 3.3 or higher for the Statistics Library)
* Boost (version 1.83 or higher for the Interpolator Module)

## Building the Toolbox

1. Create a build directory inside the root folder and navigate into it:
   ```
    mkdir build 
    cd build
   ```

2. Run CMake and build the project:

   ```
   cmake ..
   make
    ```

## Usage

The toolbox is accessed through a main program, which allows users to interact with either the Statistics or 
Interpolator module. Include the necessary headers in your project, and compile with the corresponding shared libraries.

Example of interacting with the modules:

* Interpolator module
  * Generate points for interpolation.
  * Calculate Mean Absolute Error (MAE) for various interpolators.
* Statistics module
  * Perform statistical analysis on CSV data.
  * Get statistical information of a specified column.
  * Output statistics to a file.

To start, run the compiled executable and follow the prompts to interact with each module.

## California Housing Prices Dataset Analysis
In our examination of the California Housing Prices dataset, we conducted a comprehensive statistical analysis using 
the Statistics module. Here are our findings:

### Numerical Data Statistics

#### Location (Longitude and Latitude):

* **Longitude**:
  * Mean: -119.57
  * Median: -118.49
  * Standard Deviation: 2.00348
  * Variance: 4.01394
* **Latitude**:
  * Mean: 35.6319
  * Median: 34.26
  * Standard Deviation: 2.1359
  * Variance: 4.56207

The geographical center of the dataset is reflected in the mean and median values of longitude and latitude. The 
moderate standard deviation suggests a concentration of data within a specific geographical region without significant extremes.

* **Housing Median Age**:
  * Mean: 28.6395
  * Median: 29
  * Standard Deviation: 12.5853
  * Variance: 158.389

The close values of mean and median indicate a symmetrical age distribution of houses. The relatively high standard deviation and variance suggest a wide range of house ages, indicative of diverse housing developments.

#### Housing Attributes (Total Rooms, Bedrooms, Population, Households):

* **Total Rooms**:
  * Mean: 2635.76
  * Median: 2127
  * Standard Deviation: 2181.56
* **Total Bedrooms**:
  * Mean: 537.871
  * Median: 431
  * Standard Deviation: 421.375
* **Population**:
  * Mean: 1425.48
  * Median: 1166
  * Standard Deviation: 1132.43
* **Households**:
  * Mean: 499.54
  * Median: 409
  * Standard Deviation: 382.32

There is significant variability in the sizes of houses and populations across the dataset, as indicated by the high 
means and standard deviations. The large standard deviation for total rooms highlights the diversity in house sizes.

#### Economic Indicators (Median Income and House Value):

* **Median Income**:
  * Mean: 3.87067
  * Median: 3.5348
  * Standard Deviation: 1.89978
* **Median House Value**:
  * Mean: 206856
  * Median: 179700
  * Standard Deviation: 115393

Both median income and house value exhibit a wide range in their values, denoted by high standard deviations. The disparity between mean and median house values suggests a right-skewed distribution, likely influenced by high-value properties.

#### Categorical Data Frequency Counts
* **Ocean Proximity**:
  * <1H OCEAN: 9136
  * INLAND: 6551
  * ISLAND: 5
  * NEAR BAY: 2290
  * NEAR OCEAN: 2658

'INLAND' and '<1H OCEAN' categories are most prevalent. The rare occurrence of 'ISLAND' instances (only 5) highlights its uniqueness within this dataset.

### Correlation Matrix Analysis

The correlation matrix reveals significant relationships between various numerical variables:

#### Housing Median Age and Location Coordinates:
* Longitude: -0.108
* Latitude: 0.0112

There's a negligible correlation between housing median age and both longitude and latitude, suggesting that location 
coordinates do not significantly influence the age of housing in this dataset.

#### Total Rooms and Population:
* Correlation: 0.857

A strong correlation between total rooms and population indicates that larger homes (with more rooms) tend to have higher populations, reflecting a consistent pattern in household size relative to house size.

#### Median Income and Median House Value:
* Correlation: 0.688

A significant positive correlation between median income and house value underscores the economic principle that higher-income areas often have higher-priced houses. This strong correlation may also indicate economic homogeneity within neighborhoods.


## Interpolator Module Testing

The module was tested considering different scenarios, functions and number of points. Specifically, the following exceptions were considered:
- Validated scenarios: Interpolation point out of range
- Two points with the same x-coordinate
- At least two points required for interpolation
- Minimum points needed for specific interpolator.

### Interpolator Functionality Analysis
The performance of each interpolator was evaluated across different functions (cos(2x), sin(x), x^2) and varying numbers of data points (10, 20, 30, 40, 50), 
measured by Mean Absolute Error (MAE). The MAE reflects the deviation between interpolated and actual function values, offering insights into accuracy.

#### Results: 
The following results were calculated by generating 50 points (starting from x=0, having distance 1 between each other). More information can be found on the testing function.

Testing with cos(2x) function and 50 points.
- Interpolator: LinearInterpolator, MAE: 0.0801684
- Interpolator: PolynomialInterpolator, MAE: 1.87509e-05
- Interpolator: CardinalCubicBSplineInterpolator, MAE: 0.00366666 

Testing with sin(x) function and 50 points.
- Interpolator: LinearInterpolator, MAE: 0.0723228
- Interpolator: PolynomialInterpolator, MAE: 1.22908e-05
- Interpolator: CardinalCubicBSplineInterpolator, MAE: 0.00312363 

Testing with x^2 function and 50 points.
- Interpolator: LinearInterpolator, MAE: 0.25
- Interpolator: PolynomialInterpolator, MAE: 0.00804552
- Interpolator: CardinalCubicBSplineInterpolator, MAE: 9.73862e-14
 

#### Observations:

- **LinearInterpolator:**
    - Shows increasing MAE with more complex functions and additional data points. This behavior indicates limitations in capturing non-linear relationships accurately. The linear model tends to struggle with non-linear data, leading to higher errors as complexity increases.

- **PolynomialInterpolator:**
    - Exhibits significantly lower MAE across different functions and data point variations. Notably, for the quadratic function x^2, PolynomialInterpolator achieves close-to-zero errors consistently. Polynomial interpolation is suitable for fitting polynomial functions, providing highly accurate results when the data aligns with the polynomial degree.

- **CardinalCubicBSplineInterpolator:**
    - Demonstrates a consistent and relatively low MAE across various functions and data points. This interpolator exhibits robustness in approximating functions, even with fewer data points compared to PolynomialInterpolator.

#### Function-wise Analysis:

The performance trend indicates that:
- For simpler functions (like x^2), all interpolators produce accurate results with low MAE.
- As functions grow in complexity (like sin(x) and cos(2x)), PolynomialInterpolator and CardinalCubicBSplineInterpolator outperform LinearInterpolator, displaying better fitting capabilities.
- Please note: MAE from different functions with different number of points should not be compared between each other! In fact, the y-value can be extremely different. 
This is why testing the polynomial interpolator with 50 points (ending in 50) instead of 10 points (ending in 10) gives worse result for the x^2 function


### Team Collaboration 
- Giovanni Coronica [giovanni.coronica@studenti.units.it]:
    - Developed of the Statistics Module.
    - Contributed to the design, test and development of the Interpolator module

- Thomas Rossi Mel [thomas.rossimel@studenti.units.it]:
    - Developed the Interpolator Module.
    - Contributed to the design, test and development of the Statistics module

