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


def test_derivate():
    data_points = [Point(0, 0), Point(1, 1), Point(2, 0)]

    interpolator = LinearInterpolator(data_points)
    interpolator = ExtendedInterpolator.from_interpolator(interpolator)

    x_values = [0.0, 0.5, 1.5]
    expected_derivatives = [1.0, 1.0, -1.0]  # Precomputed derivatives for the given x-values

    for i, x_value in enumerate(x_values):
        assert round(interpolator.derivative(x_value), 6) == round(expected_derivatives[i], 6), f"Derivative mismatch at x = {x_value}"

    print("Derivative tests passed successfully.")

def test_integral():
    data_points = [Point(0, 0), Point(1, 1), Point(2, -1)]

    interpolator = LinearInterpolator(data_points)
    interpolator = ExtendedInterpolator.from_interpolator(interpolator)

    start_x_values = [0.0, 1.0]
    end_x_values = [1.0, 2.0]
    expected_integrals = [0.5, 0]  # Precomputed integrals for the given x-limits

    for start, end, expected_integral in zip(start_x_values, end_x_values, expected_integrals):
        calculated_integral = interpolator.integral(start, end)
        assert round(calculated_integral, 6) == round(expected_integral, 6), f"Integral mismatch between x = {start} and {end}"

    print("Integral tests passed successfully.")

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

    print(f"Time taken by native (python) linear interpolator for 1k points: {time_native} seconds")
    print(f"Time taken by non-native (C++) LinearInterpolator for 1k points: {time_non_native} seconds")
    print(f"Time taken by scipy's linear interpolator for 1k points: {time_scipy} seconds")



if __name__ == "__main__":

    # interpolation
    compare_languages_performance()
    test_derivate()
    test_integral()

    print("All test successful, now plotting some interpolated functions. ")
    plot_interpolators()