from scipy.interpolate import Akima1DInterpolator
from interpolator_py import Point
from typing import List
from .extended_interpolator import ExtendedInterpolator


class AkimaSplineInterpolator(ExtendedInterpolator):
    def __init__(self, points: List[Point]):
        super().__init__(points)

        x = [point.x for point in points]
        y = [point.y for point in points]
        self.spline = Akima1DInterpolator(x, y)

    def __call__(self, point: float) -> float:
        if point < self.points[0].x or point > self.points[-1].x:
            raise ValueError("Interpolation point out of range")

        return self.spline(point)
