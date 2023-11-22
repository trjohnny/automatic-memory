#include "polynomial_interpolator.hpp"

PolynomialInterpolator::PolynomialInterpolator(const Eigen::VectorXd& x_vals, const Eigen::VectorXd& y_vals, int degree)
        : Interpolator(x_vals, y_vals) {
    if (x.size() <= degree || degree < 0) {
        throw std::invalid_argument("Invalid polynomial degree");
    }

    Eigen::MatrixXd A(x.size(), degree + 1);
    for (int i = 0; i < x.size(); ++i) {
        for (int j = 0; j <= degree; ++j) {
            A(i, j) = pow(x[i], j);
        }
    }

    coefficients = A.colPivHouseholderQr().solve(y);
}

double PolynomialInterpolator::operator()(double x_val) const {
    double result = 0.0;
    for (int i = 0; i <= coefficients.rows() - 1; ++i) {
        result += coefficients[i] * pow(x_val, i);
    }
    return result;
}