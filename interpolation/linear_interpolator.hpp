#ifndef LINEAR_INTERPOLATOR_HPP
#define LINEAR_INTERPOLATOR_HPP
#include "interpolator.hpp"

class LinearInterpolator : public Interpolator {
public:
    LinearInterpolator(const Eigen::VectorXd& x_vals, const Eigen::VectorXd& y_vals)
            : Interpolator(x_vals, y_vals) {}

    double operator()(double x_val) const override;
};

#endif