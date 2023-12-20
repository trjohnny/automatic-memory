from interpolation.akima_spline_interpolator import AkimaSplineInterpolator
from interpolator_py import Point

# Testing Akima spline interpolator and the plot function
data_points = [Point(1, 2), Point(2, 3), Point(3, 1), Point(4, 5), Point(5, 2)]
akima_interpolator = AkimaSplineInterpolator(data_points)
start_interpolated_point = 1
end_interpolated_point = 5
step_interpolated_point = 0.1
akima_interpolator.plot(1, 5, 0.1)
