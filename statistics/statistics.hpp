#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <vector>
#include <string>
#include <variant>
#include <optional>
#include <iostream>
#include <fstream>
#include <json/json.h>

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
    // More function declarations for statistical operations
};

#endif // STATISTICS_HPP
