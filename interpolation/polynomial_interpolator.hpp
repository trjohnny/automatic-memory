#ifndef POLYNOMIAL_INTERPOLATOR_HPP
#define POLYNOMIAL_INTERPOLATOR_HPP
#include "interpolator.hpp"

namespace scitool {
    class polynomial_interpolator : public interpolator {
    public:
        polynomial_interpolator(const std::vector<point> &points)
                : interpolator(points) {}

        double operator()(double point) const override;
    };
}


#endif