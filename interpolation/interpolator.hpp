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
    Interpolator(const std::vector<Point>& points)
            : points(points) {

        std::sort(this->points.begin(), this->points.end(), [](const Point& a, const Point& b) {
            return a.x < b.x;
        });

        for(int i = 1; i < points.size(); i++)
            if(points[i].x == points[i-1].x)
                throw std::invalid_argument("Two points have the same value for \"x\"");

        if (points.size() < 2)
            throw std::invalid_argument("At least two points are needed to perform interpolation");
    }

    virtual double operator()(double x_val) const = 0;
};

#endif
