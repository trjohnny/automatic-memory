//
// Created by Giovanni Coronica on 01/12/23.
//

#include "stat_utils.cpp"
#include "dataset.hpp"
#include <fstream>
#include <iomanip>

namespace scitool {

    std::unique_ptr<dataset> dataset::from_csv(const std::string& input_file) {
        std::ifstream file(input_file);
        if (!file.is_open()) {
            throw std::invalid_argument("Unable to open file: " + input_file);
        }

        std::vector<std::string> columns_;
        dataset::matrix data_matrix_;
        std::set<int> numerical_columns_;
        std::set<int> categorical_columns_;

        // Read header
        std::string line;
        if (std::getline(file, line)) {
            std::istringstream header_stream(line);
            std::string column;
            while (std::getline(header_stream, column, ',')) {
                columns_.push_back(column);
            }
        }

        // Read first data row and determine column types
        bool is_first_row = true;
        while (std::getline(file, line)) {
            std::istringstream line_stream(line);
            std::string cell;
            dataset::data_row data_row;
            int column_index = 0;

            while (std::getline(line_stream, cell, ',')) {
                // Convert string to data_variant
                auto data_variant = dataset::convert(cell);
                data_row.push_back(data_variant);

                // On first row, determine if the column is numerical
                if (is_first_row) {
                    if (std::holds_alternative<int>(data_variant.value()) ||
                        std::holds_alternative<double>(data_variant.value())) {
                        numerical_columns_.insert(column_index);
                    } else {
                        categorical_columns_.insert(column_index);
                    }
                }

                column_index++;
            }
            data_matrix_.push_back(std::move(data_row));
            is_first_row = false;
        }

        std::string file_name = extract_file_name(input_file);

        auto ds = std::make_unique<dataset>(std::move(columns_), std::move(data_matrix_),
                                            std::move(numerical_columns_), std::move(categorical_columns_));
        ds->file_name = std::move(file_name);
        return ds;
    }

    std::string dataset::extract_file_name(const std::string& path) {
        // Find the last '/' or '\\' character (handle both Unix and Windows paths)
        size_t last_slash = path.find_last_of("/\\");
        if (last_slash == std::string::npos) last_slash = 0;
        else last_slash++;

        // Find the last '.' character
        size_t last_dot = path.find_last_of('.');
        if (last_dot == std::string::npos || last_dot < last_slash) {
            last_dot = path.length();
        }

        // Extract file name without extension
        return path.substr(last_slash, last_dot - last_slash);
    }

    void dataset::calculate_statistics() {
        // Calculate statistics for numerical columns
        for (auto colIndex : numerical_columns) {
            const auto& columnName = columns[colIndex];
            column_statistics[columnName].mean = get_mean(columnName);
            column_statistics[columnName].std_dev = get_std_dev(columnName);
            column_statistics[columnName].variance = get_variance(columnName);
            column_statistics[columnName].median = get_median(columnName);
        }

        // Calculate the correlation matrix for numerical columns
        correlation_matrix = get_correlation_matrix();

        // Calculate frequency counts for categorical columns
        for (auto colIndex : categorical_columns) {
            const auto& columnName = columns[colIndex];
            column_statistics[columnName].frequency_count = get_frequency_count(columnName);
        }
    }

    void dataset::output_statistics(const std::string& output_file) {
        std::ofstream out_file(output_file);
        if (!out_file.is_open()) {
            throw std::runtime_error("Unable to open file: " + output_file);
        }

        calculate_statistics();

        // Output for Numerical Data
        out_file << "Numerical Data Statistics:\n";
        for (int col_index : numerical_columns) {
            auto& col_name = columns[col_index];
            out_file << col_name << ":\n";
            out_file << "  Mean: " << column_statistics[col_name].mean.value() << "\n";
            out_file << "  Median: " << column_statistics[col_name].median.value() << "\n";
            out_file << "  Standard Deviation: " << column_statistics[col_name].std_dev.value() << "\n";
            out_file << "  Variance: " << column_statistics[col_name].variance.value() << "\n";
        }

        // Output for Categorical Data
        out_file << "\nCategorical Data Frequency Counts:\n";
        for (int col_index : categorical_columns) {
            auto& col_name = columns[col_index];
            out_file << col_name << ":\n";
            for (const auto& pair : column_statistics[col_name].frequency_count.value()) {
                out_file << "  " << pair.first << ": " << pair.second << "\n";
            }
        }

        out_file << "\n\n";

        auto column_widths = dataset::get_width(columns);
        int max_row_label_width = *std::max_element(column_widths.begin(), column_widths.end());

        // Output column headers
        out_file << std::setw(max_row_label_width) << std::setfill(' ') << " |";
        for (int col_index : numerical_columns) {
            out_file << std::setw(column_widths[col_index] - 1) << columns[col_index] << "|";
        }
        out_file << "\n";

        // Output horizontal separator with '+' at intersections
        out_file << std::setfill('-') << std::setw(max_row_label_width) << "+";
        for (int col_index : numerical_columns) {
            out_file << std::setw(column_widths[col_index]) << std::setfill('-') << "+";
        }
        out_file << std::setfill(' ') << "\n";

        // Output rows with row labels
        for (Eigen::Index i = 0; i < correlation_matrix.value().rows(); ++i) {
            out_file << std::setw(max_row_label_width - 1) << columns[i] << "|";
            for (Eigen::Index j = 0; j < correlation_matrix.value().cols(); ++j) {
                out_file << std::setw(column_widths[j] - 1) << std::setprecision(3) << correlation_matrix.value()(i, j) << "|";
            }
            out_file << "\n";
        }

        out_file << std::setfill('-') << std::setw(max_row_label_width) << "+";
        for (size_t i = 0; i < numerical_columns.size(); ++i) {
            out_file << std::setw(column_widths[i]) << std::setfill('-') << "+";
        }
        out_file << std::setfill(' ') << "\n";

        out_file.close();
    }

    void dataset::calculate_mean(const std::string& column_name) {
        int col_index = column_statistics[column_name].col_index;
        std::vector<std::optional<double>> column_data = extract_numerical_column_data(col_index);
        double mean_value = scitool::mean(column_data);
        column_statistics[column_name].mean = mean_value;
    }

    void dataset::calculate_std_dev(const std::string& column_name) {
        int col_index = column_statistics[column_name].col_index;
        std::vector<std::optional<double>> column_data = extract_numerical_column_data(col_index);
        double std_dev_value = scitool::std_dev(column_data);
        column_statistics[column_name].std_dev = std_dev_value;
    }

    void dataset::calculate_median(const std::string& column_name) {
        int col_index = column_statistics[column_name].col_index;
        std::vector<std::optional<double>> column_data = extract_numerical_column_data(col_index);
        double median_value = scitool::median(column_data);
        column_statistics[column_name].median = median_value;
    }

    void dataset::calculate_variance(const std::string& column_name) {
        int col_index = column_statistics[column_name].col_index;
        std::vector<std::optional<double>> column_data = extract_numerical_column_data(col_index);
        double variance_value = scitool::variance(column_data);
        column_statistics[column_name].variance = variance_value;
    }

    void dataset::calculate_frequency_count(const std::string& column_name) {
        int col_index = column_statistics[column_name].col_index;
        std::map<std::string, int> frequency_map = extract_categorical_column_data(col_index);
        column_statistics[column_name].frequency_count = frequency_map;
    }

    // Helper function to extract numerical data from a column
    std::vector<std::optional<double>> dataset::extract_numerical_column_data(int col_index) {
        std::vector<std::optional<double>> column_data;
        column_data.reserve(data_matrix.size()); // Reserve space to avoid repeated reallocations

        for (const auto& row : data_matrix) {
            const auto& cell = row[col_index];
            if (!cell) {
                // If the cell is empty, add std::nullopt
                column_data.emplace_back(std::nullopt);
                continue;
            }

            // Extract the value if it's an int or double, else add std::nullopt
            column_data.emplace_back(
                    std::visit([](const auto& arg) -> std::optional<double> {
                        using T = std::decay_t<decltype(arg)>;
                        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
                            return static_cast<double>(arg);
                        } else {
                            return std::nullopt;
                        }
                    }, cell.value())
            );
        }

        return column_data;
    }

    // Helper function to extract categorical data from a column
    std::map<std::string, int> dataset::extract_categorical_column_data(int colIndex) {
        std::map<std::string, int> frequency_map;
        for (const auto& row : data_matrix) {
            auto& cell = row[colIndex];
            if (cell.has_value() && std::holds_alternative<std::string>(cell.value())) {
                std::string value = std::get<std::string>(cell.value());
                frequency_map[value]++;
            }
        }
        return frequency_map;
    }

    double dataset::get_mean(const std::string& column_name) {
        auto& col_stats = column_statistics[column_name];
        if (!col_stats.mean) {
            calculate_mean(column_name);
        }
        return col_stats.mean.value();
    }

    double dataset::get_variance(const std::string& column_name) {
        auto& col_stats = column_statistics[column_name];
        if (!col_stats.variance) {
            calculate_variance(column_name);
        }
        return col_stats.variance.value();
    }

    double dataset::get_std_dev(const std::string& column_name) {
        auto& col_stats = column_statistics[column_name];
        if (!col_stats.std_dev) {
            calculate_std_dev(column_name);
        }
        return col_stats.std_dev.value();
    }

    double dataset::get_median(const std::string& column_name) {
        auto& col_stats = column_statistics[column_name];
        if (!col_stats.median) {
            calculate_median(column_name);
        }
        return col_stats.median.value();
    }

    Eigen::MatrixXd dataset::get_correlation_matrix() {
        if (!correlation_matrix) {
            calculate_correlation_matrix();
        }
        return correlation_matrix.value();
    }

    std::map<std::string, int> dataset::get_frequency_count(const std::string& column_name) {
        // Check if the column exists and is categorical
        auto col_it = std::find(columns.begin(), columns.end(), column_name);
        if (col_it == columns.end()) {
            throw std::invalid_argument("Column '" + column_name + "' does not exist");
        }

        if (!is_categorical(column_name)) {
            throw std::invalid_argument("Column '" + column_name + "' is not a categorical column");
        }

        auto& col_stats = column_statistics[column_name];
        if (!col_stats.frequency_count) {
            calculate_frequency_count(column_name);
        }

        return col_stats.frequency_count.value();
    }

    void dataset::calculate_correlation_matrix() {
        size_t num_numerical_columns = numerical_columns.size();
        Eigen::MatrixXd matrix_xd(num_numerical_columns, num_numerical_columns);

        std::vector<std::vector<std::optional<double>>> column_data(num_numerical_columns);

        // Extract data for each numerical column
        int idx = 0;
        for (auto col_index : numerical_columns) {
            column_data[idx++] = extract_numerical_column_data(col_index);
        }

        for (size_t i = 0; i < num_numerical_columns; ++i) {
            for (size_t j = 0; j <= i; ++j) { // Compute only half the matrix_xd
                matrix_xd(i, j) = matrix_xd(j, i) = scitool::correlation(column_data[i], column_data[j]);
            }
        }

        correlation_matrix.emplace(matrix_xd);
    }

    bool dataset::is_categorical(const std::string& column_name) {
        auto col_it = std::find(columns.begin(), columns.end(), column_name);
        if (col_it == columns.end()) {
            throw std::invalid_argument("Column '" + column_name + "' does not exist");
        }

        auto& col_stats = column_statistics[column_name];
        return std::find(categorical_columns.begin(), categorical_columns.end(), col_stats.col_index) != categorical_columns.end();
    }

    std::optional<dataset::data_variant> dataset::convert(const std::string &str) {
        if (str.empty()) return std::nullopt;

        char* end;
        int i = (int) strtol(str.c_str(), &end, 10);
        if (*end == 0) return i;

        double d = strtod(str.c_str(), &end);
        if (*end == 0) return d;

        return str;
    }

    const std::string& dataset::get_file_name() const {
        return file_name;
    }
} // scitool