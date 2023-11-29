#include "statistics/statistics.hpp"
#include "interpolation/interpolator.hpp"
#include "interpolation/linear_interpolator.hpp"
#include "interpolation/polynomial_interpolator.hpp"
#include "interpolation/cardinal_cubic_bspline_Interpolator.hpp"
#include <iostream>
#include <string>
#include <exception>
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

int main() {

    testAllInterpolators();

    return 0;
}
