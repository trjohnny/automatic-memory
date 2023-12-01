//
// Created by Giovanni Coronica on 29/11/23.
//

#include <set>
#include <map>
#include "Eigen/Core"

#ifndef STATS_HPP
#define STATS_HPP

namespace scitool {

    class dataset;

    std::unique_ptr<dataset> read_csv(const std::string& input_file);

    template<typename T>
    static double median(const std::vector<std::optional<T>>& data);

    template<typename T>
    static double std_dev(const std::vector<std::optional<T>>& data);

    template<typename T>
    static double variance(const std::vector<std::optional<T>>& data);

    template<typename T>
    static double correlation(const std::vector<std::optional<T>>& data1, const std::vector<std::optional<T>>& data2);

    template<typename T>
    static double mean(const std::vector<std::optional<T>>& data);

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

        dataset(std::vector<std::string> cols, matrix matrix, std::set<size_t> numCols, std::set<size_t> catCols)
        : data_matrix(std::move(matrix)), columns(std::move(cols)), numerical_columns(std::move(numCols)), categorical_columns(std::move(catCols)) {
            for (size_t colIdx = 0; colIdx < columns.size(); ++colIdx) {
                column_statistics[columns[colIdx]] = column_stat{
                    .col_index = colIdx
                };
            }
        }

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

        static std::vector<int> get_width(const std::vector<std::string>& vector) {
            std::vector<int> widths;
            for (const auto& col : vector) {
                widths.push_back(static_cast<int>(col.length()) + 2);  // Additional space for padding
            }
            return widths;
        }

    };

    std::optional<dataset::data_variant> convert(const std::string &str) {
        if (str.empty()) return std::nullopt;

        char* end;
        int i = (int) strtol(str.c_str(), &end, 10);
        if (*end == 0) return i;

        double d = strtod(str.c_str(), &end);
        if (*end == 0) return d;

        return str;
    }

} // scitool

#endif //STATS_HPP
