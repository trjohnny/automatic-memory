#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/complex.h>
#include <pybind11/chrono.h>
#include "dataset.hpp"

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<std::optional<scitool::dataset::data_variant>>);
PYBIND11_MAKE_OPAQUE(std::set<int>);

PYBIND11_MODULE(statistics_py, m) {
    py::class_<scitool::dataset>(m, "Dataset")
        .def(py::init<const std::vector<std::string>&,scitool::dataset::matrix,const std::set<int>&,const std::set<int>&>())
        .def_static("from_csv", &scitool::dataset::from_csv, py::return_value_policy::take_ownership)
        .def("is_categorical", &scitool::dataset::is_categorical)
        .def("get_mean", &scitool::dataset::get_mean)
        .def("get_std_dev", &scitool::dataset::get_std_dev)
        .def("get_median", &scitool::dataset::get_median)
        .def("get_variance", &scitool::dataset::get_variance)
        .def("get_frequency_count", &scitool::dataset::get_frequency_count)
        .def("get_correlation_matrix", &scitool::dataset::get_correlation_matrix)
        .def("output_statistics", &scitool::dataset::output_statistics)
        .def("__iter__", [](scitool::dataset &v) {
            return py::make_iterator(v.begin(), v.end());
        }, py::keep_alive<0, 1>()) // Keep dataset alive while iterator is used
        .def("__repr__",
             [](const scitool::dataset &a) {
                return "<scitool.dataset with name " + a.get_file_name() + " >";
        })
        .def("map_column", [](scitool::dataset& self, const std::string& column_name, py::function func) {
            self.map_column(column_name, [&func](auto& element) {
                return func(element).template cast <decltype(element)>();
            });
        })
        .def("filter_rows", [](scitool::dataset& self, py::function filter) {
            self.filter_rows([&filter](const auto& row) {
                return filter(row).template cast<bool>();
            });
        });

    py::class_<scitool::dataset::column_stat>(m, "ColumnStat")
        .def_readwrite("col_index", &scitool::dataset::column_stat::col_index)
        .def_readwrite("mean", &scitool::dataset::column_stat::mean)
        .def_readwrite("std_dev", &scitool::dataset::column_stat::std_dev)
        .def_readwrite("median", &scitool::dataset::column_stat::median)
        .def_readwrite("variance", &scitool::dataset::column_stat::variance)
        .def_readwrite("frequency_count", &scitool::dataset::column_stat::frequency_count);


    m.attr("Eigen") = py::module_::import("numpy");
}
