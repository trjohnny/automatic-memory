#include "statistics/statistics.hpp"
#include "interpolation/interpolator.hpp"
#include "interpolation/linear_interpolator.hpp"
#include "interpolation/polynomial_interpolator.hpp"
#include "interpolation/cardinal_cubic_bspline_Interpolator.hpp"
#include <iostream>
#include <string>
#include <exception>
#include "muparser/include/muParser.h"
#include <map>

/*
int main() {
    try {
        std::string csvFilePath = "../housing.csv";
        std::string outputFilePath = "../out.txt";

        Statistics stats(csvFilePath, outputFilePath);
        stats.analyze();

        std::cout << "Statistical analysis completed. Results are in: " << outputFilePath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
*/


std::vector<Point> getUserDefinedPoints() {
    std::vector<Point> userPoints;
    std::cout << "Enter points in the format 'x y'. Enter 'q' to finish input.\n";
    while (true) {
        double x, y;
        std::string input;
        std::cin >> input;
        if (input == "q") break;
        try {
            x = std::stod(input);
            std::cin >> y;
            userPoints.emplace_back(x, y);
        } catch (const std::invalid_argument& e) {
            std::cout << "Invalid input. Enter 'x y' or 'q' to finish.\n";
        }
    }
    return userPoints;
}


void testAllInterpolators(const int minPoints = 10, const int maxPoints = 50, const int incrementNumPoints = 10) {
    // Previous testAllInterpolators implementation...
    // (No changes here)
}

int main() {
    std::map<std::string, std::unique_ptr<Interpolator>> interpolators;

    mu::Parser interpolationFunction;
    bool functionDefined = false;

    std::vector<Point> interpolationPoints;

    while (true) {
        std::cout << "Options:\n";
        std::cout << "1. Initialize Interpolation Function\n";
        std::cout << "2. Initialize Interpolator Dataset\n";
        std::cout << "3. Calculate Interpolation on a Specified Point\n";
        std::cout << "4. Run Base Tests for Interpolators\n";
        std::cout << "5. Exit\n";
        std::cout << "Choose an option: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string expression;
                std::cout << "Enter the interpolation function, use x as parameter (e.g., sin(x), x^2, cos(2*x)): ";
                std::cin.ignore();
                std::getline(std::cin, expression);
                interpolationFunction.SetExpr(expression);
                functionDefined = true;
                break;
            }
            case 2: {
                interpolationPoints = getUserDefinedPoints();
                interpolators["CardinalCubicBSplineInterpolator"] = std::make_unique<CardinalCubicBSplineInterpolator>(interpolationPoints);
                interpolators["PolynomialInterpolator"] = std::make_unique<PolynomialInterpolator>(interpolationPoints);
                interpolators["LinearInterpolator"] = std::make_unique<LinearInterpolator>(interpolationPoints);

                break;
            }
            case 3: {
                if (functionDefined && !interpolationPoints.empty()) {
                    double interpolationPoint;
                    std::cout << "Enter the point to interpolate: ";
                    std::cin >> interpolationPoint;

                    std::cout << "Interpolating at point " << interpolationPoint << std::endl;
                    for (const auto& interpolator : interpolators) {
                        double interpolatedValue = (*interpolator.second)(interpolationPoint);
                        std::cout << "Interpolator: " << interpolator.first << ", Interpolated Value: " << interpolatedValue << std::endl;
                    }
                } else {
                    std::cout << "Interpolation function or dataset is not initialized.\n";
                }
                break;
            }
            case 4: {
                testAllInterpolators();
                break;
            }
            case 5: {
                std::cout << "Exiting...\n";
                return 0;
            }
            default: {
                std::cout << "Invalid choice. Please enter a valid option.\n";
                break;
            }
        }
    }

    return 0;
}