#include <stdexcept>
#define AOC_MAIN
#include "aoc.h"

#include <algorithm>
#include <ranges>
#include <cstdint>
#include <string_view>
#include <vector>

std::vector<std::vector<int>> parse(std::string_view input) {
    std::vector<std::vector<int>> out;

    for (const char *pos = input.data(); pos < input.data() + input.size(); pos++) {

        auto &line = out.emplace_back();

        while (*pos != '\n') {
            char *end;
            const int l = std::strtol(pos, &end, 10);
            if (pos == end) {
                throw std::runtime_error("Could not parse integer!");
            }
            line.push_back(l);
            pos = end;
        }
    }

    return out;
}

bool level_is_safe(std::span<const int> level) {
    int x = level[0];
    bool increasing = true;
    bool decreasing = true;
    for (size_t i = 1; i < level.size(); i++) {
        int y = level[i];
        increasing &= x < y;
        decreasing &= x > y;

        int abs = std::abs(x - y);
        if (abs < 1 || abs > 3) {
            return false;
        }

        x = y;
    }

    return increasing != decreasing;
}

int64_t task(std::string_view input) {
    auto levels = parse(input);

    auto levels_ok = levels | std::views::transform(level_is_safe);

    int64_t num_ok = std::ranges::count(levels_ok, true);

    return num_ok;
}
