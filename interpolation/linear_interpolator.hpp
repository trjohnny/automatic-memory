#ifndef LINEAR_INTERPOLATOR_HPP
#define LINEAR_INTERPOLATOR_HPP
#include "interpolator.hpp"

class LinearInterpolator : public Interpolator {
public:
    LinearInterpolator(const std::vector<Point>& points)
            : Interpolator(points) {}

    double operator()(double point) const override;
};

#endif