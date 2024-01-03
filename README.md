# Scientific Computing Toolbox (HOMEWORK 2)
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
- Giovanni Coronica [giovanni.coronica@gmail.com]:
    - Developed of the Statistics Module.
    - Contributed to the design, test and development of the Interpolator module

- Thomas Rossi Mel [rossimelthomas@gmail.com]:
    - Developed the Interpolator Module.
    - Contributed to the design, test and development of the Statistics module

# Scientific Computing Toolbox (HOMEWORK 3)
## Prerequisites
For this third homework assignment, along with the previously required dependencies, the following are also necessary:
- Pybind11 (tested with version 2.11.1)
- Python installation (tested with version 3.11.6)
- Numpy (tested with version 1.26.2)
- Scipy (tested with version 1.11.4)
- Matplotlib (tested with version 3.8.2)

## Building the Toolbox
1. Move to the main folder containing CMakeLists.txt.
2. Run the command:
   ```
    python setup.py install
   ```
3. Test the toolbox using the notebook ```test.ipynb``` in the python folder.

## Interpolator Analysis
We'll now proceed with an analysis and review of the interpolation module.

### Features
- All previously available features are retained and functional in Python.
- New features have been added: Plotting, Derivative calculation, Integral calculation.
- Introduction of the Akima Spline interpolator.
- Inclusion of a native linear interpolator class for performance comparison with C++.

### Design Choices: Binding
1. Maintained consistent naming conventions between Python and C++ for clarity and coherence across both languages.
2. Considered class inheritance and differences between C++ and Python. For instance, the py_interpolator wrapper class and binding the `__call__` method to `operator()`.

### Design Choices: Extended Interpolator
To add functionalities (derivative, integral, and plotting) to all C++ interpolators, an ExtendedInterpolator class was introduced.
1. It extends the base Interpolator while adding necessary functionalities.
2. Added a method to convert any interpolator (e.g., linear or polynomial) to an advanced interpolator through a `from_interpolator` static method, instantiating an ExtendedInterpolator class.

### Testing and Results
All methods were thoroughly tested within the main.py file. A performance analysis yielded the following results:

- Time taken by native (Python) linear interpolator for 1k points: 1.2787 seconds
- Time taken by non-native (C++) linear interpolator for 1k points: 0.0032 seconds
- Time taken by Scipy's linear interpolator for 1k points: 0.0042 seconds

These results highlight the substantial performance advantage of C++, especially when utilizing C++'s vectors compared to Python lists. Python execution time is notably higher, approximately 433 times slower than C++. The performance improves when utilizing optimized libraries like Scipy and Numpy, leveraging C++ in the background for enhanced efficiency.

## Statistics analysis

This section provides an analysis of the performance results obtained from testing a custom C++ dataset implementation 
against Pandas operations in Python. The tests were conducted to measure the time efficiency of basic statistical 
operations and row filtering tasks on a dataset. The focus is on the design choices regarding the Python wrapper class, 
new functionalities like map and filter, and the bindings to the C++ library.

### Features

- Python Wrapper for C++ Dataset: Offers a seamless Python interface to a robust C++ dataset implementation, supporting 
statistical analysis and data manipulation methods.
- Visualization and Statistical Tools: Enables correlation matrix visualization and provides essential statistical 
functions like mean, median, and variance directly in Python.
- Performance Comparison: Includes functionality to compare the performance of dataset operations between native Python 
(Pandas) and the C++ backed Python implementation.
- Data Manipulation: Introduces map_column and filter_rows methods for dynamic data manipulation, offering a Pythonic
way to apply functions and filter data in datasets.

### Design choices: new functions in the header
The map_column and filter_rows methods are defined in the scitool::dataset header file to resolve linkage issues inherent 
with C++ templated functions when interfaced with Python using pybind11. Templated functions must be fully defined in 
every translation unit they're used in; thus, defining these functions in the header ensures that their templates are 
instantiated correctly and accessible during the binding process.

### Design choices: Bindings

1. A Python-C++ interface was created using Pybind11, a lightweight header-only library that exposes C++ types in Python 
and vice versa. This allows the existing C++ dataset implementation to be utilized directly from Python.
2. Bindings were specifically designed for the dataset operations, ensuring that the complex data structures and 
algorithms of the C++ codebase could be accessed and manipulated from Python.

### Design choices: Wrapper python class

1. The Python wrapper class PyDataset was created to provide a Pythonic interface to the underlying C++ dataset 
functionalities. It acts as a facade over the C++ implementation, making it more accessible and easier to use from Python.
2. The wrapper class ensures that data types and structures are seamlessly converted between Python and C++, abstracting 
away the complexity of direct C++ interaction. This includes handling conversions between Eigen matrices and Numpy arrays,
iterating over the dataset, and exposing property-like access to dataset characteristics.
3. Enhancements like the display_correlation_matrix method were added to integrate with Python's rich ecosystem, allowing 
for direct visualization using libraries like Matplotlib and Seaborn.

### Discussion: Ease of Use vs. Performance
* The introduction of a Python wrapper class and binding to an existing C++ implementation aimed to balance ease of use 
with the performance benefits of C++. While C++ is known for its efficiency, Python's ease of use and vast ecosystem of 
data science libraries are unparalleled.
* By providing a familiar Pythonic interface to the robust C++ dataset operations, users can leverage the speed of compiled 
code without extensive knowledge of C++ or the burden of dealing with its complexity for data manipulation and analysis tasks.
### Discussion: Performance Balance:
* The performance discrepancy noted between the custom C++ implementation (even when accessed via Python) and Pandas is a 
reflection of the highly optimized nature of Pandas, which is written in Cython and C. These optimizations are specifically 
tailored for data manipulation tasks and are the result of years of development and refinement.
* While the custom C++ implementation provides an opportunity for fine-tuned optimizations and potentially faster execution 
for certain operations, achieving and surpassing the performance of Pandas requires significant effort in optimization, 
algorithm selection, and possibly parallelization.


## Team Collaboration
### Giovanni Coronica [giovanni.coronica@gmail.com]
- Bound and extended the Statistics library.
- Contributed to the development and testing of the Interpolator module (both Python and C++).

### Thomas Rossi Mel [rossimelthomas@gmail.com]
- Bound and extended the Interpolator Module.
- Contributed to the design, testing, and development of the Statistics module (both Python and C++).