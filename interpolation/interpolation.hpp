#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include <vector>

class Interpolation {
public:
    Interpolation(const std::vector<double>& x, const std::vector<double>& y);
    double operator()(double x) const;

private:
    std::vector<double> nodes;
    std::vector<double> values;
    // Functions for linear, polynomial, and B-spline interpolation
};

#endif // INTERPOLATION_HPP
