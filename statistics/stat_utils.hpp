//
// Created by Giovanni Coronica on 29/11/23.
//

#include <set>
#include <map>
#include <optional>

#ifndef STATS_HPP
#define STATS_HPP

namespace scitool {

    template<typename T>
    static double median(const std::vector<std::optional<T>>& data);

    template<typename T>
    static double std_dev(const std::vector<std::optional<T>>& data);

    template<typename T>
    static double variance(const std::vector<std::optional<T>>& data);

    template<typename T>
    static double correlation(const std::vector<std::optional<T>>& data1, const std::vector<std::optional<T>>& data2);

    template<typename T>
    static double mean(const std::vector<std::optional<T>>& data);

} // scitool

#endif //STATS_HPP
