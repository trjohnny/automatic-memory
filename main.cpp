#include "interpolation/interpolator.hpp"
#include "interpolation/linear_interpolator.hpp"
#include "interpolation/polynomial_interpolator.hpp"
#include "interpolation/cardinal_cubic_bspline_Interpolator.hpp"
#include "statistics/stats.hpp"
#include "include/muParser.h"
#include <map>

std::vector<scitool::point> generate_points(const std::function<double(double)>& function, double start, double end, double increment) {
    if (increment <= 0) {
        throw std::invalid_argument("Increment must be a positive non-zero value.");
    }

    if (start >= end) {
        throw std::invalid_argument("Start value must be less than the end value.");
    }

    std::vector<scitool::point> points;

    for (double x = start; x < end; x += increment) {
        double y = function(x);
        points.emplace_back(x, y);
    }
    return points;
}


double calcuate_interpolator_MAE(const std::unique_ptr<scitool::interpolator>& interpolator, const std::vector<scitool::point>& test_points, const std::function<double(double)>& function) {
    double total_error = 0.0;

    for (const auto& point : test_points) {
        double interpolated_value = (*interpolator)(point.x);
        double actual_value = function(point.x);
        double error = std::abs(interpolated_value - actual_value);
        total_error += error;
    }

    return total_error / static_cast<double>(test_points.size());
}

void test_all_interpolators(const int min_points = 10, const int max_points = 50, const int increment_num_points = 10) {


    std::cout << "1) Testing exceptions" << std::endl;
    try {
        scitool::linear_interpolator interpolator({{1.0, 2.0}, {3.0, 4.0}});
        interpolator(0.5);
    } catch (const std::out_of_range& e) {
        std::cout << "Testing out_of_range exception, got: " << e.what() << std::endl;
    }

    try {
        scitool::linear_interpolator interpolator({{1.0, 2.0}, {1.0, 4.0}});
    } catch (const std::invalid_argument& e) {
        std::cout << "Testing with two points having the same x coordinate, got: " << e.what() << std::endl;
    }

    try {
        scitool::linear_interpolator interpolator({{1.0, 2.0}});
    } catch (const std::invalid_argument& e) {
        std::cout << "Testing with only one point, got: " << e.what() << std::endl;
    }

    try {
        scitool::cardinal_cubic_bspline_interpolator interpolator({{1.0, 2.0},{2.0, 2.0},{3.0, 2.0}});
    } catch (const std::invalid_argument& e) {
        std::cout << "Testing with only 3 points and the cardinal cubic B-Spline interpolator, got: " << e.what() << std::endl;
    }

    std::cout << std::endl << "2) Testing interpolator functionality" << std::endl;
    std::map<std::string, std::function<double(double)>> functions;
    functions["sin(x)"] = [](double x) { return sin(x); };
    functions["x^2"] = [](double x) { return x*x; };
    functions["cos(2x)"] = [](double x) { return cos(x); };

    for (const auto& func : functions) {
        for (int point_count = min_points; point_count <= max_points; point_count+=increment_num_points) {
            std::vector<scitool::point> points = generate_points(func.second, 0.0, point_count, 1);
            std::vector<scitool::point> test_points = generate_points(func.second, 0.5, point_count-1, 3);

            std::unordered_map<std::string, std::unique_ptr<scitool::interpolator>> interpolators;
            interpolators["CardinalCubicBSplineInterpolator"] = std::make_unique<scitool::cardinal_cubic_bspline_interpolator>(points);
            interpolators["PolynomialInterpolator"] = std::make_unique<scitool::polynomial_interpolator>(points);
            interpolators["LinearInterpolator"] = std::make_unique<scitool::linear_interpolator>(points);

            std::cout << "Testing with " << func.first << " function and " << point_count << " points." << std::endl;
            for (const auto& interpolator : interpolators) {
                std::cout << "Interpolator: " << interpolator.first  << ", MAE: " << calcuate_interpolator_MAE(interpolator.second, test_points, func.second) << std::endl;
            }

            std::cout << "\n";
        }
    }
}

std::vector<scitool::point> get_user_defined_points() {
    std::vector<scitool::point> user_points;
    std::cout << "Enter points in the format 'x y'. Enter 'q' to finish input.\n";
    while (true) {
        double x, y;
        std::string input;
        std::cin >> input;
        if (input == "q") break;
        try {
            x = std::stod(input);
            std::cin >> y;
            user_points.emplace_back(x, y);
        } catch (const std::invalid_argument& e) {
            std::cout << "Invalid input. Enter 'x y' or 'q' to finish.\n";
        }
    }
    return user_points;
}

void handle_interpolator_module() {
    std::map<std::string, std::unique_ptr<scitool::interpolator>> interpolators;

    mu::Parser interpolation_function;
    bool function_defined = false;
    double point_to_interpolate = 0;

    std::vector<scitool::point> interpolation_points;

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
                    interpolation_function.SetExpr(expression);
                    interpolation_function.DefineVar("x", &point_to_interpolate);
                    interpolation_function.Eval(); // Check if the expression can be evaluated
                    function_defined = true;
                } catch (const mu::Parser::exception_type& e) {
                    std::cout << "Parsing error in the function: " << e.GetMsg() << std::endl;
                    function_defined = false;
                }
                break;
            }
            case 2: {
                try {
                    interpolation_points = get_user_defined_points();
                    interpolators["CardinalCubicBSplineInterpolator"] = std::make_unique<scitool::cardinal_cubic_bspline_interpolator>(interpolation_points);
                    interpolators["PolynomialInterpolator"] = std::make_unique<scitool::polynomial_interpolator>(interpolation_points);
                    interpolators["LinearInterpolator"] = std::make_unique<scitool::linear_interpolator>(interpolation_points);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "An error occurred while initializing interpolators: " << e.what() << std::endl;
                }
                break;
            }
            case 3: {
                if (function_defined && !interpolation_points.empty()) {
                    try {
                        std::cout << "Enter the point to interpolate: ";
                        std::cin >> point_to_interpolate;

                        std::cout << "Interpolating at point x=" << point_to_interpolate << std::endl;
                        for (const auto& interpolator : interpolators) {
                            double interpolated_value = (*interpolator.second)(point_to_interpolate);
                            std::cout << "Interpolator: " << interpolator.first << ", Interpolated Value (estimated y): " << interpolated_value << ", Actual value (y): " << interpolation_function.Eval() <<  std::endl;
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
                test_all_interpolators();
                break;
            }
            case 5: {
                std::cout << "Exiting...\n";
                return;
            }
            default: {
                std::cout << "Invalid choice. Please enter a valid option.\n";
                break;
            }
        }
    }
}

void handle_statistics_module() {
    std::string csv_file_path, outputFilePath, column_name;
    std::unique_ptr<scitool::dataset> ds;
    int choice;
    bool fileLoaded = false;

    while (true) {
        std::cout << "\nStatistics Module:\n";
        std::cout << "1. Select CSV File\n";
        std::cout << "2. Statistical Info of a Column\n";
        std::cout << "3. Output Stats to File\n";
        std::cout << "4. Return to Main Menu\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::cout << "Enter the path to the CSV file: ";
                std::cin >> csv_file_path;
                ds = scitool::read_csv(csv_file_path);
                fileLoaded = true; // Set to true if file is successfully loaded
                break;
            }
            case 2: {
                if (!fileLoaded) {
                    std::cout << "Please load a CSV file first.\n";
                    break;
                }
                std::cout << "Enter column name for statistical info: ";
                std::cin >> column_name;
                if (!ds->is_categorical(column_name))
                {
                    std::cout << "Stats for numerical column " << column_name << " :" << std::endl;
                    double mean = ds->get_mean(column_name);
                    double median = ds->get_median(column_name);
                    double std_dev = ds->get_std_dev(column_name);
                    double variance = ds->get_variance(column_name);

                    std::cout << " - Mean = " << mean << std::endl;
                    std::cout << " - Median = " << median << std::endl;
                    std::cout << " - Standard deviation = " << std_dev << std::endl;
                    std::cout << " - Variance = " << variance << std::endl;
                }
                else
                {
                    std::cout << "Stats for categorical column " << column_name << " :" << std::endl;
                    std::map<std::string, int> frequency_count_map = ds->get_frequency_count(column_name);
                    for (const auto& pair : frequency_count_map) {
                        std::cout << " - " << pair.first << ": " << pair.second << "\n";
                    }
                }
                break;
            }
            case 3: {
                if (!fileLoaded) {
                    std::cout << "Please load a CSV file first.\n";
                    break;
                }
                std::cout << "Enter the path to the output file: ";
                std::cin >> outputFilePath;
                ds->output_statistics(outputFilePath);
                break;
            }
            case 4:
                return; // Return to the main menu
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
}

int main() {

    while (true) {
        std::cout << "Choose a module:\n";
        std::cout << "1. Statistics\n";
        std::cout << "2. Interpolator\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                handle_statistics_module();
                break;
            case 2:
                handle_interpolator_module();
                break;
            case 3:
                std::cout << "Exiting program...\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
}