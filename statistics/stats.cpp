#include "stats.hpp"
#include <fstream>
#include <numeric>

namespace scitool {

    std::unique_ptr<dataset> read_csv(const std::string& input_file) {
        std::ifstream file(input_file);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + input_file);
        }

        std::vector<std::string> columns;
        dataset::matrix data_matrix;
        std::set<size_t> numerical_columns;
        std::set<size_t> categorical_columns;

        // Read header
        std::string line;
        if (std::getline(file, line)) {
            std::istringstream header_stream(line);
            std::string column;
            while (std::getline(header_stream, column, ',')) {
                columns.push_back(column);
            }
        }

        // Read first data row and determine column types
        bool is_first_row = true;
        while (std::getline(file, line)) {
            std::istringstream line_stream(line);
            std::string cell;
            dataset::data_row data_row;
            size_t column_index = 0;

            while (std::getline(line_stream, cell, ',')) {
                // Convert string to data_variant
                auto data_variant = convert(cell);
                data_row.push_back(data_variant);

                // On first row, determine if the column is numerical
                if (is_first_row) {
                    if (std::holds_alternative<int>(data_variant.value()) ||
                        std::holds_alternative<double>(data_variant.value())) {
                        numerical_columns.insert(column_index);
                    } else {
                        categorical_columns.insert(column_index);
                    }
                }

                column_index++;
            }
            data_matrix.push_back(std::move(data_row));
            is_first_row = false;
        }

        return std::make_unique<dataset>(std::move(columns), std::move(data_matrix),
                                         std::move(numerical_columns), std::move(categorical_columns));
    }

    template<typename T>
    static double median(const std::vector<std::optional<T>>& data) { // Note: pass by value
        if (data.empty()) {
            throw std::runtime_error("Cannot compute median of an empty vector");
        }

        std::vector<std::optional<T>> sorted_data = data;

        std::sort(sorted_data.begin(), sorted_data.end());

        size_t n = sorted_data.size();
        size_t mid = n / 2;

        return n % 2 == 0 ? (sorted_data[mid].value() + sorted_data[mid - 1].value()) / 2.0 : sorted_data[mid].value();
    }


    template<typename T>
    static double std_dev(const std::vector<std::optional<T>>& data) {
        if (data.empty()) {
            throw std::runtime_error("Cannot compute standard deviation of an empty vector");
        }

        // Calculate mean, excluding std::nullopt elements
        double sum = 0.0;
        size_t count = 0;
        for (const auto& opt_val : data) {
            if (opt_val) {
                sum += static_cast<double>(*opt_val);
                ++count;
            }
        }
        if (count == 0) {
            throw std::runtime_error("Cannot compute standard deviation with all values missing");
        }
        double data_mean = sum / static_cast<double>(count);

        // Calculate squared sum of differences from the mean
        double sq_sum = std::accumulate(data.begin(), data.end(), 0.0,
                                        [data_mean](double acc, const std::optional<T>& opt_val) {
                                            return opt_val ? acc + (static_cast<double>(*opt_val) - data_mean) * (static_cast<double>(*opt_val) - data_mean) : acc;
                                        });
        return std::sqrt(sq_sum / static_cast<double>(count));
    }

    template<typename T>
    static double variance(const std::vector<std::optional<T>>& data) {
        if (data.empty()) {
            throw std::runtime_error("Cannot compute variance of an empty vector");
        }

        double std_dev = scitool::std_dev(data);

        return std_dev * std_dev;
    }

    template<typename T>
    static double correlation(const std::vector<std::optional<T>>& data1, const std::vector<std::optional<T>>& data2) {
        if (data1.empty() || data1.size() != data2.size()) {
            throw std::runtime_error("Cannot compute correlation of vectors with unequal size or empty vectors");
        }

        double mean1 = mean(data1);
        double mean2 = mean(data2);
        double sum_xx = 0, sum_yy = 0, sum_xy = 0;

        for (size_t i = 0; i < data1.size(); ++i) {
            if (!data1[i] || !data2[i]) continue;
            sum_xx += (data1[i].value() - mean1) * (data1[i].value() - mean1);
            sum_yy += (data2[i].value() - mean2) * (data2[i].value() - mean2);
            sum_xy += (data1[i].value() - mean1) * (data2[i].value() - mean2);
        }

        return sum_xy / (std::sqrt(sum_xx) * std::sqrt(sum_yy));
    }

    template<typename T>
    static double mean(const std::vector<std::optional<T>>& data) {
        if (data.empty()) {
            throw std::runtime_error("Cannot compute mean of an empty vector");
        }

        double sum = 0.0;
        size_t count = 0;
        for (const auto& optVal : data) {
            if (optVal) {
                sum += static_cast<double>(*optVal);
                ++count;
            }
        }

        if (count == 0) {
            throw std::runtime_error("Cannot compute mean with all values missing");
        }

        return sum / static_cast<double>(count);
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
        for (size_t col_index : numerical_columns) {
            auto& col_name = columns[col_index];
            out_file << col_name << ":\n";
            out_file << "  Mean: " << column_statistics[col_name].mean.value() << "\n";
            out_file << "  Median: " << column_statistics[col_name].median.value() << "\n";
            out_file << "  Standard Deviation: " << column_statistics[col_name].std_dev.value() << "\n";
            out_file << "  Variance: " << column_statistics[col_name].variance.value() << "\n";
        }

        // Output for Categorical Data
        out_file << "\nCategorical Data Frequency Counts:\n";
        for (size_t col_index : categorical_columns) {
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
        for (size_t col_index : numerical_columns) {
            out_file << std::setw(column_widths[col_index] - 1) << columns[col_index] << "|";
        }
        out_file << "\n";

        // Output horizontal separator with '+' at intersections
        out_file << std::setfill('-') << std::setw(max_row_label_width) << "+";
        for (size_t col_index : numerical_columns) {
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
        size_t col_index = column_statistics[column_name].col_index;
        printf("Calculating mean column %s at index %zu\n", column_name.c_str(), col_index);
        std::vector<std::optional<double>> column_data = extract_numerical_column_data(col_index);
        double mean_value = scitool::mean(column_data);
        column_statistics[column_name].mean = mean_value;
    }

    void dataset::calculate_std_dev(const std::string& column_name) {
        size_t col_index = column_statistics[column_name].col_index;
        std::vector<std::optional<double>> column_data = extract_numerical_column_data(col_index);
        double std_dev_value = scitool::std_dev(column_data);
        column_statistics[column_name].std_dev = std_dev_value;
    }

    void dataset::calculate_median(const std::string& column_name) {
        size_t col_index = column_statistics[column_name].col_index;
        std::vector<std::optional<double>> column_data = extract_numerical_column_data(col_index);
        double median_value = scitool::median(column_data);
        column_statistics[column_name].median = median_value;
    }

    void dataset::calculate_variance(const std::string& column_name) {
        size_t col_index = column_statistics[column_name].col_index;
        std::vector<std::optional<double>> column_data = extract_numerical_column_data(col_index);
        double variance_value = scitool::variance(column_data);
        column_statistics[column_name].variance = variance_value;
    }

    void dataset::calculate_frequency_count(const std::string& column_name) {
        size_t col_index = column_statistics[column_name].col_index;
        std::map<std::string, int> frequency_map = extract_categorical_column_data(col_index);
        column_statistics[column_name].frequency_count = frequency_map;
    }

    // Helper function to extract numerical data from a column
    std::vector<std::optional<double>> dataset::extract_numerical_column_data(size_t col_index) {
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
    std::map<std::string, int> dataset::extract_categorical_column_data(size_t colIndex) {
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
        // Check if the column is categorical
        auto col_it = std::find(columns.begin(), columns.end(), column_name);
        if (col_it == columns.end() ||
            categorical_columns.find(std::distance(columns.begin(), col_it)) == categorical_columns.end()) {
            throw std::runtime_error("Column '" + column_name + "' is not a categorical column");
        }

        auto& col_stats = column_statistics[column_name];
        if (!col_stats.median) {
            calculate_frequency_count(column_name);
        }

        return col_stats.frequency_count.value();
    }

    void dataset::calculate_correlation_matrix() {
        size_t num_numerical_columns = numerical_columns.size();
        Eigen::MatrixXd matrix_xd(num_numerical_columns, num_numerical_columns);

        std::vector<std::vector<std::optional<double>>> column_data(num_numerical_columns);

        // Extract data for each numerical column
        size_t idx = 0;
        for (auto col_index : numerical_columns) {
            column_data[idx++] = extract_numerical_column_data(col_index);
        }

        #pragma omp parallel for collapse(2) // OpenMP directive for parallel for loop
        for (size_t i = 0; i < num_numerical_columns; ++i) {
            for (size_t j = 0; j <= i; ++j) { // Compute only half the matrix_xd
                matrix_xd(i, j) = matrix_xd(j, i) = scitool::correlation(column_data[i], column_data[j]);
            }
        }

        correlation_matrix.emplace(matrix_xd);
    }
}