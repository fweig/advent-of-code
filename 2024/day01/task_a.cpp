#include <algorithm>
#include <numeric>
#include <vector>
#include <span>

#define AOC_MAIN
#include "aoc.h"


std::pair<std::vector<int>, std::vector<int>> parse(std::string_view input) {
    std::vector<int> left, right;

    for (const char *pos = input.data(); pos < input.data() + input.size(); ) {

        char *end;
        const int l = std::strtol(pos, &end, 10);
        left.push_back(l);

        pos = end;

        const int r = std::strtol(pos, &end, 10);
        right.push_back(r);

        pos = end;

        if (*pos == '\n') {
            pos++;
        }
    }

    return {left, right};
}


std::vector<int> elementwise_distance(std::span<const int> left, std::span<const int> right) {
    std::vector<int> out;
    out.reserve(left.size());

    for (size_t i = 0; i < left.size(); i++) {
        int d = std::abs(left[i] - right[i]);
        out.push_back(d);
    }

    return out;
}


int64_t task(std::string_view input) {
    auto [left, right] = parse(input);

    std::ranges::sort(left);
    trace("{}", left);

    std::ranges::sort(right);
    trace("{}", right);

    auto sum = elementwise_distance(left, right);

    auto acc = std::accumulate(sum.begin(), sum.end(), 0);

    return acc;
}
