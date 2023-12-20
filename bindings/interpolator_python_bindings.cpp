#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "interpolator.hpp"
#include "linear_interpolator.hpp"
#include "polynomial_interpolator.hpp"
#include "cardinal_cubic_bspline_Interpolator.hpp"

namespace py = pybind11;

class py_interpolator : public scitool::interpolator {
public:
    using interpolator::interpolator; // Inherit constructors

    double operator()(double x_val) const override {
        PYBIND11_OVERRIDE_PURE(double, interpolator, operator(), x_val);
    }

    std::vector<scitool::point> get_points() const {
        return points;
    }
    void set_points(std::vector<scitool::point> points) {
        this->points = points;
    }
};

PYBIND11_MODULE(interpolator_py, m) {
    py::class_<scitool::point>(m, "Point")
    .def(py::init<double, double>())
    .def_readwrite("x", &scitool::point::x)
    .def_readwrite("y", &scitool::point::y);

    py::class_<py_interpolator>(m, "Interpolator")
            .def(py::init<const std::vector<scitool::point>&>())
            .def_property("_points", &py_interpolator::get_points, &py_interpolator::set_points);

    py::class_<scitool::linear_interpolator>(m, "LinearInterpolator")
    .def(py::init<const std::vector<scitool::point>&>())
    .def("__call__", &scitool::linear_interpolator::operator());

    py::class_<scitool::polynomial_interpolator>(m, "PolynomialInterpolator")
    .def(py::init<const std::vector<scitool::point>&>())
    .def("__call__", &scitool::polynomial_interpolator::operator());

    py::class_<scitool::cardinal_cubic_bspline_interpolator>(m, "CardinalCubicBSplineInterpolator")
    .def(py::init<const std::vector<scitool::point>&>())
    .def("__call__", &scitool::cardinal_cubic_bspline_interpolator::operator());
}