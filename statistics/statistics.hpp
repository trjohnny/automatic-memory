#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <Eigen/Dense>
#include <vector>
#include <string>
#include <variant>
#include <optional>
#include <map>

class Statistics {
public:
    Statistics(const std::string& inputFile, const std::string& outputFile);
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
    std::vector<std::map<std::string, int>> categoricalFrequencyCounts;
    Eigen::MatrixXd correlationMatrix;

    void loadData();
    void calculateStatistics();
    void outputResults();
};

#endif // STATISTICS_HPP
