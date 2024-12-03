#include <algorithm>
#include <ranges>
#include <cstdint>
#include <string_view>
#include <stdexcept>
#include <vector>

#define AOC_MAIN
#include "aoc.h"


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

bool level_is_safe_tolerate(std::span<const int> level, size_t imask=-1) {

    size_t ifirst = imask == 0 ? 1 : 0;

    int x = level[ifirst];
    bool increasing = true;
    bool decreasing = true;
    for (size_t i = ifirst + 1; i < level.size(); i++) {

        if (i == imask) {
            continue;
        }

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

bool level_is_safe(std::span<const int> level) {
    if (level_is_safe_tolerate(level)) {
        return true;
    }

    for (size_t i = 0; i < level.size(); i++) {
        if (level_is_safe_tolerate(level, i)) {
            return true;
        }
    }

    return false;
}

int64_t task(std::string_view input) {
    auto levels = parse(input);

    auto levels_ok = levels | std::views::transform(level_is_safe);

    int64_t num_ok = std::ranges::count(levels_ok, true);

    return num_ok;
}
