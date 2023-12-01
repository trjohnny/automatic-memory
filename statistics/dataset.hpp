//
// Created by Giovanni Coronica on 01/12/23.
//
#include "stat_utils.hpp"
#include <set>
#include <map>
#include <optional>
#include <memory>
#include <variant>
#include "Eigen/Core"

#ifndef DATASET_HPP
#define DATASET_HPP

namespace scitool {

    class dataset {
    public:
        using data_variant = std::variant<int, double, std::string>;
        using data_row = std::vector<std::optional<data_variant>>;
        using matrix = std::vector<data_row>;

        struct column_stat {
            size_t col_index;
            std::optional<double> mean;
            std::optional<double> std_dev;
            std::optional<double> median;
            std::optional<double> variance;
            std::optional<std::map<std::string, int>> frequency_count;
        };

        class iterator {
        public:
            explicit iterator(typename matrix::iterator it) : current(it) {}

            iterator& operator++() {
                ++current;
                return *this;
            }

            bool operator!=(const iterator& other) const {
                return current != other.current;
            }

            const data_row& operator*() const {
                return *current;
            }

        private:
            typename matrix::iterator current;
        };

        dataset(std::vector<std::string> cols, matrix matrix, std::set<size_t> num_cols, std::set<size_t> cat_cols)
                : data_matrix(std::move(matrix)), columns(std::move(cols)), numerical_columns(std::move(num_cols)), categorical_columns(std::move(cat_cols)) {
            for (size_t col_idx = 0; col_idx < columns.size(); ++col_idx) {
                column_statistics[columns[col_idx]] = column_stat{};
                column_statistics[columns[col_idx]].col_index = col_idx;
            }
        }

        static std::unique_ptr<dataset> from_csv(const std::string& input_file);

        bool is_categorical(const std::string& column_name);

        double get_mean(const std::string& column_name);
        double get_std_dev(const std::string& column_name);
        double get_median(const std::string& column_name);
        double get_variance(const std::string& column_name);
        std::map<std::string, int> get_frequency_count(const std::string& column_name);
        Eigen::MatrixXd get_correlation_matrix();

        void output_statistics(const std::string& output_file);

        auto begin() {
            return data_matrix.begin();
        }

        auto end() {
            return data_matrix.end();
        }

    private:
        matrix data_matrix;
        std::vector<std::string> columns;
        std::unordered_map<std::string, column_stat> column_statistics;
        std::optional<Eigen::MatrixXd> correlation_matrix;
        std::set<size_t> numerical_columns;
        std::set<size_t> categorical_columns;

        // Helper methods to calculate statistics
        void calculate_statistics();
        void calculate_mean(const std::string& column_name);
        void calculate_std_dev(const std::string& column_name);
        void calculate_median(const std::string& column_name);
        void calculate_variance(const std::string& column_name);
        void calculate_frequency_count(const std::string& column_name); // For frequency count
        void calculate_correlation_matrix();

        std::map<std::string, int> extract_categorical_column_data(size_t colIndex);
        std::vector<std::optional<double>> extract_numerical_column_data(size_t col_index);

        static std::optional<dataset::data_variant> convert(const std::string &str);
        static std::vector<int> get_width(const std::vector<std::string>& vector) {
            std::vector<int> widths;
            for (const auto& col : vector) {
                widths.push_back(static_cast<int>(col.length()) + 2);  // Additional space for padding
            }
            return widths;
        }

    };

} // scitool

#endif //DATASET_HPP
