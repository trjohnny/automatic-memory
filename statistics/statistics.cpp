#include <fstream>
#include <gsl/gsl_statistics_double.h>
#include <gsl/gsl_sort_double.h>
#include <map>
#include "csv.hpp"
#include "statistics.hpp"

Statistics::Statistics(const std::string& inputFile, const std::string& outputFile)
        : inputFile(inputFile), outputFile(outputFile) {}

bool mightBeHeader(const csv::CSVRow& row) {
    for (auto& field : row) {
        if (!field.is_str() || !std::all_of(field.get<>().begin(), field.get<>().end(), ::isalpha)) {
            return false;
        }
    }
    return true;
}

void Statistics::loadData() {
    csv::CSVReader reader(inputFile);

    auto firstRow = reader.begin();
    bool hasHeader = firstRow != reader.end() && mightBeHeader(*firstRow);

    if (hasHeader) {
        columns = reader.get_col_names();
    } else {
        // Generate generic column names (Column1, Column2, etc.)
        for (size_t i = 0; i < firstRow->size(); ++i) {
            columns.push_back("Column" + std::to_string(i + 1));
        }
    }

    dataMatrix.resize(columns.size());

    for (auto& row : reader) {
        size_t columnIndex = 0;
        for (auto& field : row) {
            std::optional<DataVariant> value;

            if (field.is_null()) {
                value = std::nullopt; // Handle blank fields
            } else if (field.is_int()) {
                value = field.get<int>();
            } else if (field.is_float()) {
                value = field.get<double>();
            } else {
                value = field.get<>();
            }

            dataMatrix[columnIndex].push_back(value);
            columnIndex++;
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
        for (size_t j = i + 1; j < numColumns; ++j) {
            if (!preprocessedNumericalData[i].empty() && !preprocessedNumericalData[j].empty()) {
                correlationMatrix(i, j) = correlationMatrix(j, i) = gsl_stats_correlation(
                        preprocessedNumericalData[i].data(), 1,
                        preprocessedNumericalData[j].data(), 1,
                        preprocessedNumericalData[i].size());
            }
        }
    }
}


void Statistics::outputResults() {
    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        throw std::runtime_error("Unable to open file: " + outputFile);
    }

    // Output for Numerical Data
    outFile << "Numerical Data Statistics:\n";
    for (size_t i = 0; i < numericalStatistics.size(); ++i) {
        outFile << columns[i] << ":\n";
        outFile << "  Mean: " << numericalStatistics[i].mean << "\n";
        outFile << "  Median: " << numericalStatistics[i].median << "\n";
        outFile << "  Standard Deviation: " << numericalStatistics[i].stdDev << "\n";
        outFile << "  Variance: " << numericalStatistics[i].variance << "\n";
    }

    // Output for Categorical Data
    outFile << "\nCategorical Data Frequency Counts:\n";
    for (size_t i = 0; i < categoricalFrequencyCounts.size(); ++i) {
        outFile << columns[i] << ":\n";
        for (const auto& pair : categoricalFrequencyCounts[i]) {
            outFile << "  " << pair.first << ": " << pair.second << "\n";
        }
    }

    // Output Correlation Matrix
    outFile << "\nCorrelation Matrix:\n";
    for (Eigen::Index i = 0; i < correlationMatrix.rows(); ++i) {
        for (Eigen::Index j = 0; j < correlationMatrix.cols(); ++j) {
            outFile << std::setw(10) << std::setprecision(3) << correlationMatrix(i, j) << " ";
        }
        outFile << "\n";
    }

    outFile.close();
}

void Statistics::analyze() {
    loadData();
    calculateStatistics();
    outputResults();
}

// More function definitions for statistical operations
