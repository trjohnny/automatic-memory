#include "statistics.hpp"
#include <iostream>
#include <exception>

int main() {
    try {
        std::string csvFilePath = "/Users/giovanni/housing.csv";
        std::string outputFilePath = "/Users/giovanni/out.txt";

        Statistics stats(csvFilePath, outputFilePath);
        stats.analyze();

        std::cout << "Statistical analysis completed. Results are in: " << outputFilePath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
