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
    correlationMatrix = Eigen::MatrixXd(numColumns, numColumns);

    // Temporary storage for numerical data for calculations
    std::vector<double> tempNumericalData;

    for (size_t i = 0; i < numColumns; ++i) {
        tempNumericalData.clear();

        // Determine if the column is numerical or categorical
        bool isNumerical = std::holds_alternative<double>(dataMatrix[i].front().value_or(0));

        for (const auto& val : dataMatrix[i]) {
            if (val.has_value()) {
                if (isNumerical) {
                    tempNumericalData.push_back(std::get<double>(*val));
                } else {
                    std::string category = std::get<std::string>(*val);
                    categoricalFrequencyCounts[i][category]++;
                }
            }
        }

        if (isNumerical) {
            // Calculate numerical statistics using GSL
            size_t dataSize = tempNumericalData.size();
            gsl_sort(tempNumericalData.data(), 1, dataSize);
            numericalStatistics[i].mean = gsl_stats_mean(tempNumericalData.data(), 1, dataSize);
            numericalStatistics[i].median = gsl_stats_median_from_sorted_data(tempNumericalData.data(), 1, dataSize);
            numericalStatistics[i].variance = gsl_stats_variance(tempNumericalData.data(), 1, dataSize);
            numericalStatistics[i].stdDev = gsl_stats_sd(tempNumericalData.data(), 1, dataSize);
        }
    }

    // Calculate correlation matrix
    for (size_t i = 0; i < numColumns; ++i) {
        for (size_t j = 0; j < numColumns; ++j) {
            if (i != j && std::holds_alternative<double>(dataMatrix[i].front().value_or(0))
                && std::holds_alternative<double>(dataMatrix[j].front().value_or(0))) {
                std::vector<double> columnJData;
                for (const auto& val : dataMatrix[j]) {
                    if (val.has_value()) {
                        columnJData.push_back(std::get<double>(*val));
                    }
                }
                correlationMatrix(i, j) = gsl_stats_correlation(
                        tempNumericalData.data(), 1,
                        columnJData.data(), 1,
                        tempNumericalData.size());
            } else {
                correlationMatrix(i, j) = std::numeric_limits<double>::quiet_NaN();
            }
        }
    }
}

void Statistics::outputResults() {
    // Implementation to output results to outputFile
}

void Statistics::analyze() {
    loadData();
    calculateStatistics();
    outputResults();
}

// More function definitions for statistical operations
