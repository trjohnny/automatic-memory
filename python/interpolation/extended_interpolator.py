import numpy as np
from interpolator_py import Interpolator
import matplotlib.pyplot as plt

# Advanced interpolator base class that integrates the c++ interpolator.
class ExtendedInterpolator(Interpolator):
    def plot(self):
        x_points = [point.x for point in self.points]
        y_points = [point.y for point in self.points]
        x_interpolated = np.arange(x_points[0], x_points[-1], 0.001)
        y_interpolated = np.array([self(i) for i in x_interpolated.tolist()])

        plt.figure(figsize=(8, 6))
        plt.scatter(x_points, y_points, color='blue', label='Original Points')
        plt.plot(x_interpolated, y_interpolated, color='red', label='Interpolated Points')
        plt.xlabel('X-axis')
        plt.ylabel('Y-axis')
        plt.title('Interpolation')
        plt.legend()
        plt.grid(True)
        plt.show()

    def __len__(self):
        return len(self.points)

    def derivative(self, x_value):
        epsilon = 1e-6
        f_x = self(x_value)
        f_x_plus_epsilon = self(x_value + epsilon)
        return (f_x_plus_epsilon - f_x) / epsilon

    def integral(self, start_x, end_x, num_samples=1000):
        if start_x >= end_x:
            raise ValueError("Start x-value should be less than the end x-value.")

        x_values = np.linspace(start_x, end_x, num_samples)
        y_values = [self(x) for x in x_values]

        integral_value = np.trapz(y_values, x=x_values)
        return integral_value

    # For new python interpolators (like Akime) we can extend this class,
    # but for c++ interpolator classes, we cannot extend it or modify the code.
    # So a from_interpolator becomes useful
    @classmethod
    def from_interpolator(cls, interpolator):
        class NewExtendedInterpolator(cls):
            def __init__(self, interpolator):
                super().__init__(interpolator.points)
                self.interpolator = interpolator
            def __call__(self, point: float):
                return self.interpolator(point)

        new_interpolator = NewExtendedInterpolator(interpolator)
        return new_interpolator