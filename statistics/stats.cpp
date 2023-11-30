#include "stats.hpp"
#include "Eigen/Core"
#include <fstream>
#include <sstream>
#include <utility>
#include <set>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <map>

namespace Stats {

    std::unique_ptr<Dataset> read_csv(const std::string& inputFile) {
        std::ifstream file(inputFile);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + inputFile);
        }

        std::vector<std::string> columns;
        Dataset::DataMatrix dataMatrix;
        std::set<size_t> numericalColumns;
        std::set<size_t> categoricalColumns;

        // Read header
        std::string line;
        if (std::getline(file, line)) {
            std::istringstream headerStream(line);
            std::string column;
            while (std::getline(headerStream, column, ',')) {
                columns.push_back(column);
            }
        }

        // Read first data row and determine column types
        bool isFirstRow = true;
        while (std::getline(file, line)) {
            std::istringstream lineStream(line);
            std::string cell;
            Dataset::DataRow currentRow;
            size_t columnIndex = 0;

            while (std::getline(lineStream, cell, ',')) {
                // Convert string to DataVariant
                auto dataVariant = convert(cell);
                currentRow.push_back(dataVariant);

                // On first row, determine if the column is numerical
                if (isFirstRow) {
                    if (std::holds_alternative<int>(dataVariant.value()) ||
                        std::holds_alternative<double>(dataVariant.value())) {
                        numericalColumns.insert(columnIndex);
                    } else {
                        categoricalColumns.insert(columnIndex);
                    }
                }

                columnIndex++;
            }
            dataMatrix.push_back(std::move(currentRow));
            isFirstRow = false;
        }

        return std::make_unique<Dataset>(std::move(columns), std::move(dataMatrix),
                                         std::move(numericalColumns), std::move(categoricalColumns));
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
        for (const auto& optVal : data) {
            if (optVal) {
                sum += static_cast<double>(*optVal);
                ++count;
            }
        }
        if (count == 0) {
            throw std::runtime_error("Cannot compute standard deviation with all values missing");
        }
        double dataMean = sum / count;

        // Calculate squared sum of differences from the mean
        double sq_sum = std::accumulate(data.begin(), data.end(), 0.0,
                                        [dataMean](double acc, const std::optional<T>& optVal) {
                                            return optVal ? acc + (static_cast<double>(*optVal) - dataMean) * (static_cast<double>(*optVal) - dataMean) : acc;
                                        });
        return std::sqrt(sq_sum / count);
    }



    template<typename T>
    static double variance(const std::vector<std::optional<T>>& data) {
        if (data.empty()) {
            throw std::runtime_error("Cannot compute variance of an empty vector");
        }

        double std_dev = Stats::std_dev(data);

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

        return sum / count;
    }

    void Dataset::calculateStatistics() {
        // Calculate statistics for numerical columns
        for (auto colIndex : numericalColumns) {
            const auto& columnName = columns[colIndex];
            columnStats[columnName].mean = getMean(columnName);
            columnStats[columnName].stdDev = getStdDev(columnName);
            columnStats[columnName].variance = getVariance(columnName);
            columnStats[columnName].median = getMedian(columnName);
        }

        // Calculate the correlation matrix for numerical columns
        correlationMatrix = getCorrelationMatrix();

        // Calculate frequency counts for categorical columns
        for (auto colIndex : categoricalColumns) {
            const auto& columnName = columns[colIndex];
            columnStats[columnName].frequencyCount = getFrequencyCount(columnName);
        }
    }

    void Dataset::output_statistics(const std::string& outputFile) {
        std::ofstream outFile(outputFile);
        if (!outFile.is_open()) {
            throw std::runtime_error("Unable to open file: " + outputFile);
        }

        calculateStatistics();

        // Output for Numerical Data
        outFile << "Numerical Data Statistics:\n";
        for (size_t col_index : numericalColumns) {
            auto& col_name = columns[col_index];
            outFile << col_name << ":\n";
            outFile << "  Mean: " << columnStats[col_name].mean.value() << "\n";
            outFile << "  Median: " << columnStats[col_name].median.value() << "\n";
            outFile << "  Standard Deviation: " << columnStats[col_name].stdDev.value() << "\n";
            outFile << "  Variance: " << columnStats[col_name].variance.value() << "\n";
        }

        // Output for Categorical Data
        outFile << "\nCategorical Data Frequency Counts:\n";
        for (size_t col_index : categoricalColumns) {
            auto& col_name = columns[col_index];
            outFile << col_name << ":\n";
            for (const auto& pair : columnStats[col_name].frequencyCount.value()) {
                outFile << "  " << pair.first << ": " << pair.second << "\n";
            }
        }

        outFile << "\n\n";

        auto columnWidths = Dataset::getWidth(columns);
        int maxRowLabelWidth = *std::max_element(columnWidths.begin(), columnWidths.end());

        // Output column headers
        outFile << std::setw(maxRowLabelWidth) << std::setfill(' ') << " |";
        for (size_t col_index : numericalColumns) {
            outFile << std::setw(columnWidths[col_index]-1) << columns[col_index] << "|";
        }
        outFile << "\n";

        // Output horizontal separator with '+' at intersections
        outFile << std::setfill('-') << std::setw(maxRowLabelWidth) << "+";
        for (size_t col_index : numericalColumns) {
            outFile << std::setw(columnWidths[col_index]) << std::setfill('-') << "+";
        }
        outFile << std::setfill(' ') << "\n";

        // Output rows with row labels
        for (Eigen::Index i = 0; i < correlationMatrix.value().rows(); ++i) {
            outFile << std::setw(maxRowLabelWidth - 1) << columns[i] << "|";
            for (Eigen::Index j = 0; j < correlationMatrix.value().cols(); ++j) {
                outFile << std::setw(columnWidths[j] - 1) << std::setprecision(3) << correlationMatrix.value()(i, j) << "|";
            }
            outFile << "\n";
        }

        outFile << std::setfill('-') << std::setw(maxRowLabelWidth) << "+";
        for (size_t i = 0; i < numericalColumns.size(); ++i) {
            outFile << std::setw(columnWidths[i]) << std::setfill('-') << "+";
        }
        outFile << std::setfill(' ') << "\n";

        outFile.close();
    }

    void Dataset::calculateMean(const std::string& columnName) {
        size_t colIndex = columnStats[columnName].col_index;
        printf("Calculating mean column %s at index %zu\n", columnName.c_str(), colIndex);
        std::vector<std::optional<double>> columnData = extractNumericalColumnData(colIndex);
        double meanValue = Stats::mean(columnData);
        columnStats[columnName].mean = meanValue;
    }

    void Dataset::calculateStdDev(const std::string& columnName) {
        size_t colIndex = columnStats[columnName].col_index;
        std::vector<std::optional<double>> columnData = extractNumericalColumnData(colIndex);
        double stdDevValue = Stats::std_dev(columnData);
        columnStats[columnName].stdDev = stdDevValue;
    }

    void Dataset::calculateMedian(const std::string& columnName) {
        size_t colIndex = columnStats[columnName].col_index;
        std::vector<std::optional<double>> columnData = extractNumericalColumnData(colIndex);
        double medianValue = Stats::median(columnData);
        columnStats[columnName].median = medianValue;
    }

    void Dataset::calculateVariance(const std::string& columnName) {
        size_t colIndex = columnStats[columnName].col_index;
        std::vector<std::optional<double>> columnData = extractNumericalColumnData(colIndex);
        double varianceValue = Stats::variance(columnData);
        columnStats[columnName].variance = varianceValue;
    }

    void Dataset::calculateFrequencyCount(const std::string& columnName) {
        size_t colIndex = columnStats[columnName].col_index;
        std::map<std::string, int> frequencyMap = extractCategoricalColumnData(colIndex);
        columnStats[columnName].frequencyCount = frequencyMap;
    }

    // Helper function to extract numerical data from a column
    std::vector<std::optional<double>> Dataset::extractNumericalColumnData(size_t colIndex) {
        std::vector<std::optional<double>> columnData;
        columnData.reserve(dataMatrix.size()); // Reserve space to avoid repeated reallocations

        for (const auto& row : dataMatrix) {
            const auto& cell = row[colIndex];
            if (!cell) {
                // If the cell is empty, add std::nullopt
                columnData.emplace_back(std::nullopt);
                continue;
            }

            // Extract the value if it's an int or double, else add std::nullopt
            columnData.emplace_back(
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

        return columnData;
    }

    // Helper function to extract categorical data from a column
    std::map<std::string, int> Dataset::extractCategoricalColumnData(size_t colIndex) {
        std::map<std::string, int> frequencyMap;
        for (const auto& row : dataMatrix) {
            auto& cell = row[colIndex];
            if (cell.has_value() && std::holds_alternative<std::string>(cell.value())) {
                std::string value = std::get<std::string>(cell.value());
                frequencyMap[value]++;
            }
        }
        return frequencyMap;
    }

    double Dataset::getMean(const std::string& columnName) {
        auto& colStats = columnStats[columnName];
        if (!colStats.mean) {
            calculateMean(columnName);
        }
        return colStats.mean.value();
    }

    double Dataset::getVariance(const std::string& columnName) {
        auto& colStats = columnStats[columnName];
        if (!colStats.variance) {
            calculateVariance(columnName);
        }
        return colStats.variance.value();
    }

    double Dataset::getStdDev(const std::string& columnName) {
        auto& colStats = columnStats[columnName];
        if (!colStats.stdDev) {
            calculateStdDev(columnName);
        }
        return colStats.stdDev.value();
    }

    double Dataset::getMedian(const std::string& columnName) {
        auto& colStats = columnStats[columnName];
        if (!colStats.median) {
            calculateMedian(columnName);
        }
        return colStats.median.value();
    }

    Eigen::MatrixXd Dataset::getCorrelationMatrix() {
        if (!correlationMatrix) {
            calculateCorrelationMatrix();
        }
        return correlationMatrix.value();
    }

    std::map<std::string, int> Dataset::getFrequencyCount(const std::string& columnName) {
        // Check if the column is categorical
        auto colIt = std::find(columns.begin(), columns.end(), columnName);
        if (colIt == columns.end() ||
            categoricalColumns.find(std::distance(columns.begin(), colIt)) == categoricalColumns.end()) {
            throw std::runtime_error("Column '" + columnName + "' is not a categorical column");
        }

        size_t colIndex = std::distance(columns.begin(), colIt);
        return extractCategoricalColumnData(colIndex);
    }

    void Dataset::calculateCorrelationMatrix() {
        size_t numNumericalColumns = numericalColumns.size();
        Eigen::MatrixXd matrix(numNumericalColumns, numNumericalColumns);

        std::vector<std::vector<std::optional<double>>> columnData(numNumericalColumns);

        // Extract data for each numerical column
        size_t idx = 0;
        for (auto colIndex : numericalColumns) {
            columnData[idx++] = extractNumericalColumnData(colIndex);
        }

        // Calculate pairwise correlations
        #pragma omp parallel for collapse(2) // OpenMP directive for parallel for loop
        for (size_t i = 0; i < numNumericalColumns; ++i) {
            for (size_t j = 0; j <= i; ++j) { // Compute only half the matrix
                matrix(i, j) = matrix(j, i) = Stats::correlation(columnData[i], columnData[j]);
            }
        }

        correlationMatrix = matrix;
    }

}
