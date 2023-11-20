#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <boost/json.hpp>
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
    std::vector<std::variant<int, double, std::string>> data;
    std::string inputFile;
    std::string outputFile;

    void loadData();
    void calculateStatistics();
    void outputResults();
    boost::json::value toJSON();

    // Utility functions to convert data to Eigen types
    Eigen::VectorXd convertToEigenVector(const std::vector<double>& values);
};

#endif // STATISTICS_HPP
