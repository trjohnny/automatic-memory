from abc import abstractmethod
from typing import List
# Pythonic way to implement the linear interpolator
# This file is used only to test and compare the performance with C++
class Point:
    def __init__(self, x: float, y: float):
        self.x = x
        self.y = y

class Interpolator:
    def __init__(self, points: List[Point]):
        self.points = sorted(points, key=lambda p: p.x)
        if len(self.points) < 2:
            raise ValueError("At least two points are needed to perform interpolation")
        for i in range(1, len(self.points)):
            if self.points[i].x == self.points[i - 1].x:
                raise ValueError("Two points have the same value for 'x'")

    @abstractmethod
    def __call__(self, x_val: float) -> float:
        pass

class LinearInterpolator(Interpolator):
    def __call__(self, x_val: float) -> float:
        if x_val < self.points[0].x or x_val > self.points[-1].x:
            raise ValueError("Interpolation point out of range")

        for i in range(len(self.points) - 1):
            if self.points[i].x <= x_val <= self.points[i + 1].x:
                slope = (self.points[i + 1].y - self.points[i].y) / (self.points[i + 1].x - self.points[i].x)
                return self.points[i].y + slope * (x_val - self.points[i].x)

        return 0.0