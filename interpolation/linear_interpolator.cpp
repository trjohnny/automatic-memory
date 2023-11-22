#include "linear_interpolator.hpp"

double LinearInterpolator::operator()(double x_val) const  {
    if (x_val < x[0] || x_val > x[x.size() - 1]) {
        throw std::out_of_range("Interpolation point out of range");
    }

    for (int i = 0; i < x.size() - 1; ++i) {
        if (x_val >= x[i] && x_val <= x[i + 1]) {
            double slope = (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
            return y[i] + slope * (x_val - x[i]);
        }
    }
    throw std::logic_error("Interpolation failed");
}