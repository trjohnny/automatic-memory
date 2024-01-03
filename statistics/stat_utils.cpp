#include "stat_utils.hpp"
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <numeric>

namespace scitool {

    template<typename T>
    static double median(const std::vector<std::optional<T>>& data) {
        // Filter out non-engaged optionals and copy engaged values to a new vector
        std::vector<T> values;
        for (const auto& opt : data) {
            if (opt) values.push_back(*opt);
        }

        if (values.empty()) {
            throw std::runtime_error("Cannot compute median of an empty or fully non-engaged optional vector");
        }

        // Sort the vector of engaged values
        std::sort(values.begin(), values.end());

        size_t n = values.size();
        size_t mid = n / 2;

        // Calculate median based on odd or even size
        return n % 2 == 0 ? (values[mid] + values[mid - 1]) / 2.0 : values[mid];
    }


    template<typename T>
    static double std_dev(const std::vector<std::optional<T>>& data) {
        if (data.empty()) {
            throw std::runtime_error("Cannot compute standard deviation of an empty vector");
        }

        // Calculate mean, excluding std::nullopt elements
        double sum = 0.0;
        size_t count = 0;
        for (const auto& opt_val : data) {
            if (opt_val) {
                sum += static_cast<double>(*opt_val);
                ++count;
            }
        }
        if (count == 0) {
            throw std::runtime_error("Cannot compute standard deviation with all values missing");
        }
        double data_mean = sum / static_cast<double>(count);

        // Calculate squared sum of differences from the mean
        double sq_sum = std::accumulate(data.begin(), data.end(), 0.0,
                                        [data_mean](double acc, const std::optional<T>& opt_val) {
                                            return opt_val ? acc + (static_cast<double>(*opt_val) - data_mean) * (static_cast<double>(*opt_val) - data_mean) : acc;
                                        });
        return std::sqrt(sq_sum / static_cast<double>(count));
    }

    template<typename T>
    static double variance(const std::vector<std::optional<T>>& data) {
        if (data.empty()) {
            throw std::runtime_error("Cannot compute variance of an empty vector");
        }

        double std_dev = scitool::std_dev(data);

        return std_dev * std_dev;
    }

    template<typename T>
    static double correlation(const std::vector<std::optional<T>>& data1, const std::vector<std::optional<T>>& data2) {
        if (data1.empty() || data1.size() != data2.size()) {
            throw std::runtime_error("Cannot compute correlation of vectors with unequal size or empty vectors");
        }

        double mean1 = mean(data1);
        double mean2 = mean(data2);
        double sum_xx = 0, sum_yy = 0, sum_xy = 0;

        for (size_t i = 0; i < data1.size(); ++i) {
            if (!data1[i] || !data2[i]) continue;
            sum_xx += (data1[i].value() - mean1) * (data1[i].value() - mean1);
            sum_yy += (data2[i].value() - mean2) * (data2[i].value() - mean2);
            sum_xy += (data1[i].value() - mean1) * (data2[i].value() - mean2);
        }

        return sum_xy / (std::sqrt(sum_xx) * std::sqrt(sum_yy));
    }

    template<typename T>
    static double mean(const std::vector<std::optional<T>>& data) {
        if (data.empty()) {
            throw std::runtime_error("Cannot compute mean of an empty vector");
        }

        double sum = 0.0;
        size_t count = 0;
        for (const auto& optVal : data) {
            if (optVal) {
                sum += static_cast<double>(*optVal);
                ++count;
            }
        }

        if (count == 0) {
            throw std::runtime_error("Cannot compute mean with all values missing");
        }

        return sum / static_cast<double>(count);
    }
}