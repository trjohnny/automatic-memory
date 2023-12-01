#ifndef LINEAR_INTERPOLATOR_HPP
#define LINEAR_INTERPOLATOR_HPP
#include "interpolator.hpp"

namespace scitool {
    class linear_interpolator : public interpolator {
    public:
        linear_interpolator(const std::vector<point> &points)
                : interpolator(points) {}

        double operator()(double point) const override;
    };
}
#endif