#include "polynomial_interpolator.hpp"

double PolynomialInterpolator::operator()(double point) const  {

    if (point < points[0].x || point > points[points.size() - 1].x) {
        throw std::out_of_range("Interpolation point out of range");
    }

    double result = 0.0;

    for (int i = 0; i < (int)points.size(); ++i) {
        double term = points[i].y;
        for (int j = 0; j <  (int)points.size(); ++j) {
            if (j != i) {
                term *= (point - points[j].x) / (points[i].x - points[j].x);
            }
        }
        result += term;
    }

    return result;
}