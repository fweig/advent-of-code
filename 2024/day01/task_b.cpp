#define AOC_MAIN
#include "aoc.h"

#include <numeric>
#include <vector>
#include <span>


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


std::vector<int> count_elements(std::span<const int> data, const int max) {
    std::vector<int> elem_count(max, 0);

    for (auto x : data) {
        elem_count.at(x)++;
    }

    return elem_count;
}


void apply_similarity_score(std::span<int> data, std::span<const int> elem_count) {
    for (auto &x : data) {
        x *= elem_count[x];
    }
}


int64_t task(std::string_view input) {
    auto [left, right] = parse(input);

    auto elem_count = count_elements(right, 100'000);

    apply_similarity_score(left, elem_count);

    auto acc = std::reduce(left.begin(), left.end());

    return acc;
}
