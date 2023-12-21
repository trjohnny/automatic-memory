#ifndef INTERPOLATOR_HPP
#define INTERPOLATOR_HPP

#include <vector>
#include <iostream>
#include <algorithm>

namespace scitool {

    struct point {
        double x, y;

        point(double x_val, double y_val) : x(x_val), y(y_val) {}
    };

    class interpolator {
    protected:
        std::vector<point> points;

    public:
        interpolator(const std::vector<point> &);
        virtual ~interpolator() = default;
        virtual double operator()(double x_val) const = 0;

        const std::vector<scitool::point>& get_points() const {
            return points;
        }
    };

}

#endif
