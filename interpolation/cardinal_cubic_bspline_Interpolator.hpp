#ifndef CARDINAL_CUBIC_B_SPLINE_INTERPOLATOR_HPP
#define CARDINAL_CUBIC_B_SPLINE_INTERPOLATOR_HPP
#include "interpolator.hpp"
#include <boost/math/interpolators/cardinal_cubic_b_spline.hpp>

namespace scitool {
    class cardinal_cubic_bspline_interpolator : public interpolator {
    private:
        boost::math::interpolators::cardinal_cubic_b_spline<double> spline;
    public:
        cardinal_cubic_bspline_interpolator(const std::vector<point> &points);

        double operator()(double point) const override;
    };
}


#endif