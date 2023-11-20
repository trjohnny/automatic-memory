#include "statistics.hpp"
#include <fstream>
#include <boost/json/src.hpp>
#include "csv.hpp"

Statistics::Statistics(const std::string& inputFile, const std::string& outputFile)
        : inputFile(inputFile), outputFile(outputFile) {}

bool mightBeHeader(const csv::CSVRow& row) {
    for (const auto& field : row) {
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
            } else if (field.is_double()) {
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
    // Implementation of statistical calculations
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
