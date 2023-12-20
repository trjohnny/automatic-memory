#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "interpolator.hpp"
#include "linear_interpolator.hpp"
#include "polynomial_interpolator.hpp"
#include "cardinal_cubic_bspline_Interpolator.hpp"

namespace py = pybind11;

PYBIND11_MODULE(interpolator_py, m) {
    py::class_<scitool::point>(m, "Point")
    .def(py::init<double, double>())
    .def_readwrite("x", &scitool::point::x)
    .def_readwrite("y", &scitool::point::y);

    py::class_<scitool::interpolator, std::shared_ptr<scitool::interpolator>>(m, "Interpolator")
    .def("__call__", &scitool::interpolator::operator());

    py::class_<scitool::linear_interpolator, scitool::interpolator, std::shared_ptr<scitool::linear_interpolator>>(m, "LinearInterpolator")
    .def(py::init<const std::vector<scitool::point>&>())
    .def("__call__", &scitool::linear_interpolator::operator());

    py::class_<scitool::polynomial_interpolator, scitool::interpolator, std::shared_ptr<scitool::polynomial_interpolator>>(m, "PolynomialInterpolator")
    .def(py::init<const std::vector<scitool::point>&>())
    .def("__call__", &scitool::polynomial_interpolator::operator());

    py::class_<scitool::cardinal_cubic_bspline_interpolator, scitool::interpolator, std::shared_ptr<scitool::cardinal_cubic_bspline_interpolator>>(m, "CardinalCubicBSplineInterpolator")
    .def(py::init<const std::vector<scitool::point>&>())
    .def("__call__", &scitool::cardinal_cubic_bspline_interpolator::operator());
}