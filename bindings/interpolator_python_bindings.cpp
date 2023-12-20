#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "interpolator.hpp"
#include "linear_interpolator.hpp"
#include "polynomial_interpolator.hpp"
#include "cardinal_cubic_bspline_interpolator.hpp"

namespace py = pybind11;
using namespace scitool;

PYBIND11_MODULE(interpolator_py, m) {
    py::class_<point>(m, "Point")
    .def(py::init<double, double>())
    .def_readwrite("x", &point::x)
    .def_readwrite("y", &point::y);

    py::class_<interpolator, std::shared_ptr<interpolator>>(m, "Interpolator")
    .def("__call__", &interpolator::operator());

    py::class_<linear_interpolator, interpolator, std::shared_ptr<linear_interpolator>>(m, "LinearInterpolator")
    .def(py::init<const std::vector<point>&>())
    .def("__call__", &linear_interpolator::operator());

    py::class_<polynomial_interpolator, interpolator, std::shared_ptr<polynomial_interpolator>>(m, "PolynomialInterpolator")
    .def(py::init<const std::vector<point>&>())
    .def("__call__", &polynomial_interpolator::operator());

    py::class_<cardinal_cubic_bspline_interpolator, interpolator, std::shared_ptr<cardinal_cubic_bspline_interpolator>>(m, "CardinalCubicBSplineInterpolator")
    .def(py::init<const std::vector<point>&>())
    .def("__call__", &cardinal_cubic_bspline_interpolator::operator());
}