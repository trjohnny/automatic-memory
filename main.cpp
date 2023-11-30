#include "interpolation/interpolator.hpp"
#include "interpolation/linear_interpolator.hpp"
#include "interpolation/polynomial_interpolator.hpp"
#include "interpolation/cardinal_cubic_bspline_Interpolator.hpp"
#include <iostream>
#include <string>
#include <exception>
#include "include/muParser.h"
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
std::vector<Point> generatePoints(const std::function<double(double)>& function, double start, double end, double increment) {
    if (increment <= 0) {
        throw std::invalid_argument("Increment must be a positive non-zero value.");
    }

    if (start >= end) {
        throw std::invalid_argument("Start value must be less than the end value.");
    }

    std::vector<Point> points;

    for (double x = start; x < end; x += increment) {
        double y = function(x);
        points.emplace_back(x, y);
    }
    return points;
}


double calculateInterpolatorMAE(const std::unique_ptr<Interpolator>& interpolator, const std::vector<Point>& testPoints, const std::function<double(double)>& function) {
    double totalError = 0.0;

    for (const auto& point : testPoints) {
        double interpolatedValue = (*interpolator)(point.x);
        double actualValue = function(point.x);
        double error = std::abs(interpolatedValue - actualValue);
        totalError += error;
    }

    return totalError / static_cast<double>(testPoints.size());
}

void testAllInterpolators(const int minPoints = 10, const int maxPoints = 50, const int incrementNumPoints = 10) {


    std::cout << "1) Testing exceptions" << std::endl;
    try {
        LinearInterpolator linearInterpolator({{1.0, 2.0}, {3.0, 4.0}});
        linearInterpolator(0.5);
    } catch (const std::out_of_range& e) {
        std::cout << "Testing out_of_range exception, got: " << e.what() << std::endl;
    }

    try {
        LinearInterpolator linearInterpolator({{1.0, 2.0}, {1.0, 4.0}});
    } catch (const std::invalid_argument& e) {
        std::cout << "Testing with two points having the same x coordinate, got: " << e.what() << std::endl;
    }

    try {
        LinearInterpolator linearInterpolator({{1.0, 2.0}});
    } catch (const std::invalid_argument& e) {
        std::cout << "Testing with only one point, got: " << e.what() << std::endl;
    }

    try {
        CardinalCubicBSplineInterpolator bSplineInterpolator({{1.0, 2.0},{2.0, 2.0},{3.0, 2.0}});
    } catch (const std::invalid_argument& e) {
        std::cout << "Testing with only 3 points and the cardinal cubic B-Spline interpolator, got: " << e.what() << std::endl;
    }

    std::cout << std::endl << "2) Testing interpolator functionality" << std::endl;
    std::map<std::string, std::function<double(double)>> functions;
    functions["sin(x)"] = [](double x) { return sin(x); };
    functions["x^2"] = [](double x) { return x*x; };
    functions["cos(2x)"] = [](double x) { return cos(x); };

    for (const auto& func : functions) {
        for (int pointCount = minPoints; pointCount <= maxPoints; pointCount+=incrementNumPoints) {
            std::vector<Point> points = generatePoints(func.second, 0.0, pointCount, 1);
            std::vector<Point> testPoints = generatePoints(func.second, 0.5, pointCount-1, 3);

            std::unordered_map<std::string, std::unique_ptr<Interpolator>> interpolators;
            interpolators["CardinalCubicBSplineInterpolator"] = std::make_unique<CardinalCubicBSplineInterpolator>(points);
            interpolators["PolynomialInterpolator"] = std::make_unique<PolynomialInterpolator>(points);
            interpolators["LinearInterpolator"] = std::make_unique<LinearInterpolator>(points);

            std::cout << "Testing with " << func.first << " function and " << pointCount << " points." << std::endl;
            for (const auto& interpolator : interpolators) {
                std::cout << "Interpolator: " << interpolator.first  << ", MAE: " << calculateInterpolatorMAE(interpolator.second, testPoints, func.second) << std::endl;
            }

            std::cout << "\n";
        }
    }
}

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

int main() {
    std::map<std::string, std::unique_ptr<Interpolator>> interpolators;

    mu::Parser interpolationFunction;
    bool functionDefined = false;
    double pointToInterpolate = 0;

    std::vector<Point> interpolationPoints;

    while (true) {
        std::cout << "Options:\n";
        std::cout << "1. Initialize Interpolation Function\n";
        std::cout << "2. Initialize Interpolator dataset\n";
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
                try {
                    interpolationFunction.SetExpr(expression);
                    interpolationFunction.DefineVar("x", &pointToInterpolate);
                    interpolationFunction.Eval(); // Check if the expression can be evaluated
                    functionDefined = true;
                } catch (const mu::Parser::exception_type& e) {
                    std::cout << "Parsing error in the function: " << e.GetMsg() << std::endl;
                }
                break;
            }
            case 2: {
                try {
                    interpolationPoints = getUserDefinedPoints();
                    interpolators["CardinalCubicBSplineInterpolator"] = std::make_unique<CardinalCubicBSplineInterpolator>(interpolationPoints);
                    interpolators["PolynomialInterpolator"] = std::make_unique<PolynomialInterpolator>(interpolationPoints);
                    interpolators["LinearInterpolator"] = std::make_unique<LinearInterpolator>(interpolationPoints);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "An error occurred while initializing interpolators: " << e.what() << std::endl;
                }
                break;
            }
            case 3: {
                if (functionDefined && !interpolationPoints.empty()) {
                    try {
                        std::cout << "Enter the point to interpolate: ";
                        std::cin >> pointToInterpolate;

                        std::cout << "Interpolating at point x=" << pointToInterpolate << std::endl;
                        for (const auto& interpolator : interpolators) {
                            double interpolatedValue = (*interpolator.second)(pointToInterpolate);
                            std::cout << "Interpolator: " << interpolator.first << ", Interpolated Value (estimated y): " << interpolatedValue << ", Actual value (y): " << interpolationFunction.Eval() <<  std::endl;
                        }
                    } catch (const std::out_of_range& e) {
                        std::cerr << "An error occurred while interpolating: " << e.what() << std::endl;
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
}