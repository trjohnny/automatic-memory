#ifndef INTERPOLATOR_HPP
#define INTERPOLATOR_HPP

#include <Eigen/Dense>

class Interpolator {
protected:
    Eigen::VectorXd x, y;

public:
    Interpolator(const Eigen::VectorXd& x_vals, const Eigen::VectorXd& y_vals)
            : x(x_vals), y(y_vals) {
        if (x.size() != y.size()) {
            throw std::invalid_argument("x and y dimensions do not match");
        }
    }

    virtual double operator()(double x_val) const = 0;
};

#endif
