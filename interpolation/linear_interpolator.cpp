#include "linear_interpolator.hpp"

double LinearInterpolator::operator()(double point) const  {
    if (point < points[0].x || point > points[points.size() - 1].x) {
        throw std::out_of_range("Interpolation point out of range");
    }

    for (int i = 0; i < (int)points.size() - 1; i++) {
        if (point >= points[i].x && point <= points[i + 1].x) {
            double slope = (points[i + 1].y - points[i].y) / (points[i + 1].x - points[i].x);
            return points[i].y + slope * (point - points[i].x);
        }
    }

    return 0.0;
}