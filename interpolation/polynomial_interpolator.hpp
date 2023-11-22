#ifndef POLYNOMIAL_INTERPOLATOR_HPP
#define POLYNOMIAL_INTERPOLATOR_HPP
#include "interpolator.hpp"

class PolynomialInterpolator : public Interpolator {
public:
    PolynomialInterpolator(const std::vector<Point>& points)
            : Interpolator(points) {}
    double operator()(double point) const override;
};


#endif