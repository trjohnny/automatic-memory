#include <fstream>
#include <utility>
#include <gsl/gsl_statistics_double.h>
#include <gsl/gsl_sort_double.h>
#include "statistics.hpp"

Statistics::Statistics(std::string  inputFile, std::string  outputFile)
        : inputFile(std::move(inputFile)), outputFile(std::move(outputFile)) {}

void Statistics::loadData() {
    std::ifstream file(inputFile);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + inputFile);
    }

    // Read header
    std::string line;
    if (std::getline(file, line)) {
        std::istringstream headerStream(line);
        std::string column;
        while (std::getline(headerStream, column, ',')) {
            columns.push_back(column);
            dataMatrix.emplace_back(); // Create a column vector for each header
        }
    }

    // Read data
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string cell;
        size_t columnIndex = 0;
        while (std::getline(lineStream, cell, ',')) {
            if (columnIndex < dataMatrix.size()) {
                dataMatrix[columnIndex].push_back(convert(cell));
                columnIndex++;
            }
        }
    }
}




void Statistics::calculateStatistics() {
    size_t numColumns = dataMatrix.size();
    numericalStatistics.resize(numColumns);
    categoricalFrequencyCounts.resize(numColumns);
    correlationMatrix = Eigen::MatrixXd::Constant(numColumns, numColumns, std::numeric_limits<double>::quiet_NaN());

    std::vector<std::vector<double>> preprocessedNumericalData(numColumns);

    // Preprocess data: separate numerical and categorical data, and check column types
    for (size_t i = 0; i < numColumns; ++i) {
        bool isNumerical = std::holds_alternative<double>(dataMatrix[i].front().value_or(0));
        if (isNumerical) {
            numericalColumns.insert(columns[i]);
        }

        for (const auto& val : dataMatrix[i]) {
            if (!val.has_value()) continue;

            if (isNumerical) {
                preprocessedNumericalData[i].push_back(std::get<double>(*val));
            } else {
                std::string category = std::get<std::string>(*val);
                categoricalFrequencyCounts[i][category]++;
            }
        }
    }

    // Calculate statistics for numerical data
    for (size_t i = 0; i < numColumns; ++i) {
        if (!preprocessedNumericalData[i].empty()) {
            size_t dataSize = preprocessedNumericalData[i].size();
            gsl_sort(preprocessedNumericalData[i].data(), 1, dataSize);

            numericalStatistics[i].mean = gsl_stats_mean(preprocessedNumericalData[i].data(), 1, dataSize);
            numericalStatistics[i].median = gsl_stats_median_from_sorted_data(preprocessedNumericalData[i].data(), 1, dataSize);
            numericalStatistics[i].stdDev = gsl_stats_sd(preprocessedNumericalData[i].data(), 1, dataSize);
            numericalStatistics[i].variance = numericalStatistics[i].stdDev * numericalStatistics[i].stdDev;
        }
    }

    // Calculate correlation matrix (upper triangle only)
    for (size_t i = 0; i < numColumns; ++i) {
        for (size_t j = i; j < numColumns; ++j) {
            if (!preprocessedNumericalData[i].empty() && !preprocessedNumericalData[j].empty()) {
                correlationMatrix(i, j) = correlationMatrix(j, i) = gsl_stats_correlation(
                        preprocessedNumericalData[i].data(), 1,
                        preprocessedNumericalData[j].data(), 1,
                        preprocessedNumericalData[i].size());
            }
        }
    }
}

std::vector<int> Statistics::getColumnWidths(const std::vector<std::string>& columns) {
    std::vector<int> widths;
    for (const auto& col : columns) {
        widths.push_back(static_cast<int>(col.length()) + 2);  // Additional space for padding
    }
    return widths;
}


void Statistics::outputResults() {
    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        throw std::runtime_error("Unable to open file: " + outputFile);
    }

    // Output for Numerical Data
    outFile << "Numerical Data Statistics:\n";
    for (size_t i = 0; i < numericalStatistics.size(); ++i) {
        if (numericalColumns.find(columns[i]) == numericalColumns.end()) continue;
        outFile << columns[i] << ":\n";
        outFile << "  Mean: " << numericalStatistics[i].mean << "\n";
        outFile << "  Median: " << numericalStatistics[i].median << "\n";
        outFile << "  Standard Deviation: " << numericalStatistics[i].stdDev << "\n";
        outFile << "  Variance: " << numericalStatistics[i].variance << "\n";
    }

    // Output for Categorical Data
    outFile << "\nCategorical Data Frequency Counts:\n";
    for (size_t i = 0; i < categoricalFrequencyCounts.size(); ++i) {
        if (numericalColumns.find(columns[i]) != numericalColumns.end()) continue;
        outFile << columns[i] << ":\n";
        for (const auto& pair : categoricalFrequencyCounts[i]) {
            outFile << "  " << pair.first << ": " << pair.second << "\n";
        }
    }

    outFile << "\n\n";

    auto columnWidths = getColumnWidths(columns);
    int maxRowLabelWidth = *std::max_element(columnWidths.begin(), columnWidths.end());

    // Output column headers
    outFile << std::setw(maxRowLabelWidth) << std::setfill(' ') << " |";
    for (size_t i = 0; i < columns.size(); ++i) {
        if (numericalColumns.find(columns[i]) == numericalColumns.end()) continue;
        outFile << std::setw(columnWidths[i]-1) << columns[i] << "|";
    }
    outFile << "\n";

    // Output horizontal separator with '+' at intersections
    outFile << std::setfill('-') << std::setw(maxRowLabelWidth) << "+";
    for (size_t i = 0; i < columns.size(); ++i) {
        if (numericalColumns.find(columns[i]) == numericalColumns.end()) continue;
        outFile << std::setw(columnWidths[i]) << std::setfill('-') << "+";
    }
    outFile << std::setfill(' ') << "\n";

    // Output rows with row labels
    for (Eigen::Index i = 0; i < correlationMatrix.rows(); ++i) {
        if (numericalColumns.find(columns[i]) == numericalColumns.end()) continue;
        outFile << std::setw(maxRowLabelWidth - 1) << columns[i] << "|";
        for (Eigen::Index j = 0; j < correlationMatrix.cols(); ++j) {
            if (numericalColumns.find(columns[j]) == numericalColumns.end()) continue;
            outFile << std::setw(columnWidths[j] - 1) << std::setprecision(3) << correlationMatrix(i, j) << "|";
        }
        outFile << "\n";
    }

    outFile << std::setfill('-') << std::setw(maxRowLabelWidth) << "+";
    for (size_t i = 0; i < columns.size(); ++i) {
        if (numericalColumns.find(columns[i]) == numericalColumns.end()) continue;
        outFile << std::setw(columnWidths[i]) << std::setfill('-') << "+";
    }
    outFile << std::setfill(' ') << "\n";

    outFile.close();
}

void Statistics::analyze() {
    loadData();
    calculateStatistics();
    outputResults();
}

std::optional<Statistics::DataVariant> Statistics::convert(const std::string &str) {
    if (str.empty()) return std::nullopt;

    char* end;
    int i = (int) strtol(str.c_str(), &end, 10);
    if (*end == 0) return i;

    double d = strtod(str.c_str(), &end);
    if (*end == 0) return d;

    return str;
}