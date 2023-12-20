from interpolation.akima_spline_interpolator import AkimaSplineInterpolator
import interpolation.native_linear_interpolator as native
from interpolator_py import LinearInterpolator
from interpolator_py import Point
import time

# Testing Akima spline interpolator and the plot function
def test_akima_interpolator():
    data_points = [Point(1, 2), Point(2, 3), Point(3, 1), Point(4, 5), Point(5, 2)]
    akima_interpolator = AkimaSplineInterpolator(data_points)
    akima_interpolator.plot(1, 5, 0.1)

def compare_languages_performance():
    num_points = 1000  # You can adjust this number as needed
    points = [Point(x, x * x) for x in range(num_points)]
    x_values = [float(x) for x in range(num_points)]

    start_time_native = time.time()
    native_interpolator = native.LinearInterpolator(points)
    for x_val in x_values:
        try:
            native_interpolator(x_val)
        except ValueError as e:
            pass
    end_time_native = time.time()
    time_native = end_time_native - start_time_native

    start_time_non_native = time.time()
    non_native_interpolator = LinearInterpolator(points)
    for x_val in x_values:
        try:
            non_native_interpolator(x_val)
        except ValueError as e:
            pass
    end_time_non_native = time.time()
    time_non_native = end_time_non_native - start_time_non_native

    print(f"Time taken by native (python) linear interpolator: {time_native} seconds")
    print(f"Time taken by non-native (c++) LinearInterpolator: {time_non_native} seconds")


if __name__ == "__main__":
    compare_languages_performance()
    test_akima_interpolator()