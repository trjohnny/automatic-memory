#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <Eigen/Dense>
#include <vector>
#include <string>
#include <variant>
#include <optional>
#include <unordered_map>
#include <unordered_set>

class Statistics {
public:
    Statistics(std::string  inputFile, std::string  outputFile);
    void analyze();

private:
    using DataVariant = std::variant<int, double, std::string>;
    std::vector<std::vector<std::optional<DataVariant>>> dataMatrix;
    std::vector<std::string> columns;
    std::string inputFile;
    std::string outputFile;

    struct NumericalStats {
        double mean;
        double median;
        double stdDev;
        double variance;
    };

    std::vector<NumericalStats> numericalStatistics;
    std::vector<std::unordered_map<std::string, int>> categoricalFrequencyCounts;
    std::unordered_set<std::string> numericalColumns;
    Eigen::MatrixXd correlationMatrix;

    void loadData();
    static std::optional<DataVariant> convert(const std::string& str);
    static std::vector<int> getColumnWidths(const std::vector<std::string>& columns);
    void calculateStatistics();
    void outputResults();
};

#endif // STATISTICS_HPP
