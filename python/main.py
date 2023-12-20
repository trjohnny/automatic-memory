from interpolation.akima_spline_interpolator import AkimaSplineInterpolator
import interpolation.native_linear_interpolator as native
from interpolator_py import LinearInterpolator
from interpolator_py import Point
import numpy as np
from scipy.interpolate import interp1d
import time

# Testing Akima spline interpolator and the plot function
def test_akima_interpolator():
    data_points = [Point(1, 2), Point(2, 3), Point(3, 1), Point(4, 5), Point(5, 2)]
    akima_interpolator = AkimaSplineInterpolator(data_points)
    akima_interpolator.plot(1, 5, 0.1)

def compare_languages_performance():
    num_points = 1000
    points = [Point(x, x * x) for x in range(num_points)]
    x_values = np.arange(num_points, dtype=float)

    # Time measurement for native (python) linear interpolator
    start_time_native = time.time()
    native_interpolator = native.LinearInterpolator(points)
    for x_val in x_values:
        native_interpolator(x_val)

    end_time_native = time.time()
    time_native = end_time_native - start_time_native

    # Time measurement for non-native (C++) LinearInterpolator
    start_time_non_native = time.time()
    non_native_interpolator = LinearInterpolator(points)
    for x_val in x_values:
        non_native_interpolator(x_val)
    end_time_non_native = time.time()
    time_non_native = end_time_non_native - start_time_non_native

    # Time measurement for scipy's linear interpolator
    x = np.array([point.x for point in points])
    y = np.array([point.y for point in points])
    start_time_scipy = time.time()
    scipy_interpolator = interp1d(x, y, kind='linear')
    for x_val in x_values:
        scipy_interpolator(x_val)
    end_time_scipy = time.time()
    time_scipy = end_time_scipy - start_time_scipy

    print(f"Time taken by native (python) linear interpolator: {time_native} seconds")
    print(f"Time taken by non-native (C++) LinearInterpolator: {time_non_native} seconds")
    print(f"Time taken by scipy's linear interpolator: {time_scipy} seconds")



if __name__ == "__main__":
    compare_languages_performance()
    test_akima_interpolator()