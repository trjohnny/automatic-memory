#ifndef INTERPOLATOR_HPP
#define INTERPOLATOR_HPP

#include <vector>
#include <iostream>
#include <algorithm>



struct Point {
    double x, y;
    Point(double x_val, double y_val) : x(x_val), y(y_val) {}
};

class Interpolator {
protected:
    std::vector<Point> points;

public:
    Interpolator(const std::vector<Point>&);
    virtual double operator()(double x_val) const = 0;
};

#endif
