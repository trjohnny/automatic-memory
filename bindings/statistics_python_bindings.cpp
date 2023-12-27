#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/complex.h>
#include <pybind11/eigen.h>
#include <pybind11/chrono.h>
#include "dataset.hpp"

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<std::optional<scitool::dataset::data_variant>>)

PYBIND11_MODULE(statistics_py, m) {
    py::class_<scitool::dataset>(m, "Dataset")
        .def(py::init<const std::vector<std::string>&,scitool::dataset::matrix,const std::set<int>&,const std::set<int>&>())
        .def_static("from_csv", &scitool::dataset::from_csv, py::return_value_policy::take_ownership)
        .def("is_categorical", &scitool::dataset::is_categorical, "Method to check if a column is categorical.")
        .def("mean", &scitool::dataset::get_mean, "Method to get the mean value of a numerical column.")
        .def("std_dev", &scitool::dataset::get_std_dev, "Method to get the standard deviation of a numerical column.")
        .def("median", &scitool::dataset::get_median, "Method to get the median of a numerical column.")
        .def("variance", &scitool::dataset::get_variance, "Method to get the variance of a numerical column.")
        .def("frequency_count", &scitool::dataset::get_frequency_count, "Method to get the frequency count of the values of a categorical column.")
            .def_property_readonly("correlation_matrix", [](scitool::dataset& v) {
                Eigen::MatrixXd matrix = v.get_correlation_matrix(); // assuming this returns an Eigen matrix
                return matrix; // pybind11 automatically converts Eigen matrices to NumPy arrays
            })
        .def("output_statistics", &scitool::dataset::output_statistics, "Outputs statistics to a text file.")
        .def_property_readonly("file_name", &scitool::dataset::get_file_name)
        .def("__len__", [](const scitool::dataset &v) {
            return v.size();
        })
        .def("__getitem__", [](const scitool::dataset &v, size_t i) {
            if (i >= v.size()) throw py::index_error();
            const auto& row = v[i];
            py::list row_list;
            for (const auto& opt_elem : row) {
                if (opt_elem) {
                    row_list.append(std::visit([](const auto& val) -> py::object {
                        using T = std::decay_t<decltype(val)>;
                        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
                            return py::cast(val);
                        } else if constexpr (std::is_same_v<T, std::string>) {
                            return py::cast(val);
                        } else {
                            throw std::runtime_error("Unsupported type in variant");
                        }}, *opt_elem));
                } else {
                    row_list.append(py::none());
                }
            }
            return row_list;
        })
        .def("__repr__",
             [](const scitool::dataset &a) {
                return "<scitool.dataset with name " + a.get_file_name() + " >";
        })
        .def("map_column", [](scitool::dataset& self, const std::string& column_name, py::function func) {
            self.map_column(column_name, [&func](double value) -> double {
                return func(value).cast<double>();
            });
        }, "Applies a function to all the values of the specified column.")
        .def("filter_rows", [](scitool::dataset& self, const std::string& column_name, py::function func) {
            self.filter_rows(column_name, [&func](double value) -> bool {
                return func(value).cast<bool>();
            });
        }, "Filters the dataset in place given a filter function and a column to filter on.");

    py::class_<scitool::dataset::column_stat>(m, "ColumnStat")
        .def_readwrite("col_index", &scitool::dataset::column_stat::col_index)
        .def_readwrite("mean", &scitool::dataset::column_stat::mean)
        .def_readwrite("std_dev", &scitool::dataset::column_stat::std_dev)
        .def_readwrite("median", &scitool::dataset::column_stat::median)
        .def_readwrite("variance", &scitool::dataset::column_stat::variance)
        .def_readwrite("frequency_count", &scitool::dataset::column_stat::frequency_count);


    m.attr("Eigen") = py::module_::import("numpy");
}
