#include "cardinal_cubic_bspline_Interpolator.hpp"

CardinalCubicBSplineInterpolator::CardinalCubicBSplineInterpolator(const std::vector<Point>& points)
        : Interpolator(points) {

    if (points.size() < 5)
        throw std::invalid_argument("At least five points are needed to perform the Cardinal Cubic interpolation");

    double distance = points[1].x - points[0].x;
    std::vector<double> function_values {points[0].y, points[1].y};

    for (int i = 2; i < (int)points.size(); i++) {
        if (points[i].x - points[i - 1].x != distance) {
            throw std::invalid_argument("Points do not have equal distance");
        }
        function_values.push_back(points[i].y);
    }

    spline = boost::math::interpolators::cardinal_cubic_b_spline<double>(
            function_values.begin(), function_values.end(), points[0].x, distance
    );

}

double CardinalCubicBSplineInterpolator::operator()(double point) const  {

    if (point < points[0].x || point > points[points.size() - 1].x) {
        throw std::out_of_range("Interpolation point out of range");
    }

    return spline(point);
}