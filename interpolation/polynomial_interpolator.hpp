#ifndef POLYNOMIAL_INTERPOLATOR_HPP
#define POLYNOMIAL_INTERPOLATOR_HPP
#include "interpolator.hpp"

class PolynomialInterpolator : public Interpolator {
private:
    Eigen::VectorXd  coefficients;

public:
    PolynomialInterpolator(const Eigen::VectorXd& x_vals, const Eigen::VectorXd& y_vals, int degree);
    double operator()(double x_val) const override;
};


#endif