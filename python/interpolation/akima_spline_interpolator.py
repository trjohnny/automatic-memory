import numpy as np
from scipy.interpolate import Akima1DInterpolator
from interpolator_py import Interpolator
import matplotlib.pyplot as plt

class AkimaSplineInterpolator(Interpolator):
    def __init__(self, points):
        super().__init__(points)
        if len(points) < 5:
            raise ValueError("At least five points are needed for Akima Spline interpolation")

        x = [point.x for point in points]
        y = [point.y for point in points]
        self.spline = Akima1DInterpolator(x, y)

    def __call__(self, point):
        if point < self.points[0].x or point > self.points[-1].x:
            raise ValueError("Interpolation point out of range")

        return self.spline(point)


    def integral(self, start, end):
        return self.spline.integrate(start, end)

    def derivative(self, point, n=1):
        return self.spline.derivative(n)(point)

    def plot(self, start_interpolated_point, end_interpolated_point, step_interpolated_point):
        x_points = [point.x for point in self._points]
        y_points = [point.y for point in self._points]
        x_interpolated = np.arange(start_interpolated_point, end_interpolated_point, step_interpolated_point)
        y_interpolated = self.spline(x_interpolated)

        plt.figure(figsize=(8, 6))
        plt.scatter(x_points, y_points, color='blue', label='Original Points')
        plt.plot(x_interpolated, y_interpolated, color='red', label='Interpolated Points')
        plt.xlabel('X-axis')
        plt.ylabel('Y-axis')
        plt.title('Akima Spline Interpolation')
        plt.legend()
        plt.grid(True)
        plt.show()