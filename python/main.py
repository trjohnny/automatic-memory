from interpolation.akima_spline_interpolator import AkimaSplineInterpolator, ExtendedInterpolator
import interpolation.native_linear_interpolator as native
from interpolator_py import LinearInterpolator, Point
import numpy as np
from scipy.interpolate import interp1d
import time

def measure_time_decorator(func):
    def wrapper(*args, **kwargs):
        start_time_native = time.time()
        func(*args, **kwargs)
        end_time_native = time.time()

        return end_time_native - start_time_native
    return wrapper

# Testing Akima spline interpolator and the plot function
def plot_interpolators():
    data_points = [Point(1, 2), Point(2, 3), Point(3, 1), Point(4, 5), Point(5, 2)]
    interpolator = AkimaSplineInterpolator(data_points)
    interpolator.plot()

    interpolator = LinearInterpolator(data_points) 
    interpolator = ExtendedInterpolator.from_interpolator(interpolator)
    interpolator.plot()


def compare_languages_performance():
    def execute_interpolation(interpolator, x_values):
        for x_val in x_values:
            interpolator(x_val)

    num_points = 1000
    points = [Point(x, x * x) for x in range(num_points)]
    x_values = np.arange(num_points, dtype=float)
    x = np.array([point.x for point in points])
    y = np.array([point.y for point in points])

    # Time measurement for native (python) linear interpolator
    time_native = measure_time_decorator(execute_interpolation)(native.LinearInterpolator(points), x_values)
    time_non_native = measure_time_decorator(execute_interpolation)(LinearInterpolator(points), x_values)
    time_scipy = measure_time_decorator(execute_interpolation)(interp1d(x, y, kind='linear'), x_values)

    print(f"Time taken by native (python) linear interpolator: {time_native} seconds")
    print(f"Time taken by non-native (C++) LinearInterpolator: {time_non_native} seconds")
    print(f"Time taken by scipy's linear interpolator: {time_scipy} seconds")



if __name__ == "__main__":
    compare_languages_performance()
    plot_interpolators()