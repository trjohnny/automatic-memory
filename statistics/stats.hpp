//
// Created by Giovanni Coronica on 29/11/23.
//
#include <string>
#include <set>
#include <unordered_map>
#include <map>
#include <optional>
#include <variant>
#include <vector>
#include <string>
#include "Eigen/Core"

#ifndef STATS_HPP
#define STATS_HPP

namespace Stats {

    class Dataset;

    std::unique_ptr<Dataset> read_csv(const std::string& path);

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

    class Dataset {
    public:
        using DataVariant = std::variant<int, double, std::string>;
        using DataRow = std::vector<std::optional<DataVariant>>;
        using DataMatrix = std::vector<DataRow>;

        struct ColumnStats {
            size_t col_index;
            std::optional<double> mean;
            std::optional<double> stdDev;
            std::optional<double> median;
            std::optional<double> variance;
            std::optional<std::map<std::string, int>> frequencyCount;
        };

        class Iterator {
        public:
            Iterator(typename DataMatrix::iterator it) : current(it) {}

            Iterator& operator++() {
                ++current;
                return *this;
            }

            bool operator!=(const Iterator& other) const {
                return current != other.current;
            }

            const DataRow& operator*() const {
                return *current;
            }

        private:
            typename DataMatrix::iterator current;
        };

        Dataset(std::vector<std::string> cols, DataMatrix matrix, std::set<size_t> numCols, std::set<size_t> catCols)
        : dataMatrix(std::move(matrix)), columns(std::move(cols)), numericalColumns(std::move(numCols)), categoricalColumns(std::move(catCols)) {
            for (size_t colIdx = 0; colIdx < columns.size(); ++colIdx) {
                columnStats[columns[colIdx]] = ColumnStats{
                    .col_index = colIdx
                };
            }
        }

        double getMean(const std::string& columnName);
        double getStdDev(const std::string& columnName);
        double getMedian(const std::string& columnName);
        double getVariance(const std::string& columnName);
        std::map<std::string, int> getFrequencyCount(const std::string& columnName);
        Eigen::MatrixXd getCorrelationMatrix();
        void output_statistics(const std::string& path_to_file);

        Iterator begin() {
            return Iterator(dataMatrix.begin());
        }

        Iterator end() {
            return Iterator(dataMatrix.end());
        }

    private:
        DataMatrix dataMatrix;
        std::vector<std::string> columns;
        std::unordered_map<std::string, ColumnStats> columnStats;
        std::optional<Eigen::MatrixXd> correlationMatrix;
        std::set<size_t> numericalColumns;
        std::set<size_t> categoricalColumns;

        // Helper methods to calculate statistics
        void calculateStatistics();
        void calculateMean(const std::string& columnName);
        void calculateStdDev(const std::string& columnName);
        void calculateMedian(const std::string& columnName);
        void calculateVariance(const std::string& columnName);
        void calculateFrequencyCount(const std::string& columnName); // For frequency count
        void calculateCorrelationMatrix();

        std::map<std::string, int> extractCategoricalColumnData(size_t colIndex);
        std::vector<std::optional<double>> extractNumericalColumnData(size_t colIndex);

        std::vector<int> getWidth(const std::vector<std::string>& vector) {
            std::vector<int> widths;
            for (const auto& col : vector) {
                widths.push_back(static_cast<int>(col.length()) + 2);  // Additional space for padding
            }
            return widths;
        }

    };

    static std::optional<Dataset::DataVariant> convert(const std::string &str) {
        if (str.empty()) return std::nullopt;

        char* end;
        int i = (int) strtol(str.c_str(), &end, 10);
        if (*end == 0) return i;

        double d = strtod(str.c_str(), &end);
        if (*end == 0) return d;

        return str;
    }

} // Stats

#endif //STATS_HPP
