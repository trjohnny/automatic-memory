#include "interpolator.hpp"

Interpolator::Interpolator(const std::vector<Point>& points)
        : points(points) {

    std::sort(this->points.begin(), this->points.end(), [](const Point& a, const Point& b) {
        return a.x < b.x;
    });

    for(int i = 1; i < (int)points.size(); i++)
        if(points[i].x == points[i-1].x)
            throw std::invalid_argument("Two points have the same value for \"x\"");

    if (points.size() < 2)
        throw std::invalid_argument("At least two points are needed to perform interpolation");
}