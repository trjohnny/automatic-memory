#ifndef CARDINAL_CUBIC_B_SPLINE_INTERPOLATOR_HPP
#define CARDINAL_CUBIC_B_SPLINE_INTERPOLATOR_HPP
#include "interpolator.hpp"
#include <boost/math/interpolators/cardinal_cubic_b_spline.hpp>

class CardinalCubicBSplineInterpolator  : public Interpolator {
private:
    boost::math::interpolators::cardinal_cubic_b_spline<double> spline;
public:
    CardinalCubicBSplineInterpolator(const std::vector<Point>& points);
    double operator()(double point) const override;
};


#endif