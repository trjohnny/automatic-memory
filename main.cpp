#include "statistics.hpp"
#include <iostream>
#include <exception>

int main() {
    try {
        std::string csvFilePath = "path/to/your/data.csv";
        std::string outputFilePath = "path/to/output/results.txt";

        Statistics stats(csvFilePath, outputFilePath);
        stats.analyze();

        std::cout << "Statistical analysis completed. Results are in: " << outputFilePath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
