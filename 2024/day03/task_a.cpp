
#include <numeric>
#include <ranges>
#include <string_view>
#include <vector>

#define AOC_MAIN
#include "aoc.h"

struct multiply_instr {
    int x;
    int y;
};

std::vector<multiply_instr> parse(std::string_view input) {
    std::vector<multiply_instr> out;

    while (!input.empty()) {
        if (!consume(input, "mul(")) {
            input.remove_prefix(1);
            continue;
        }

        auto [x, ok] = consume_int(input);
        if (!ok) {
            continue;
        }

        if (!consume(input, ",")) {
            continue;
        }

        auto [y, ok2] = consume_int(input);
        if (!ok2) {
            continue;
        }

        if (!consume(input, ")")) {
            continue;
        }

        out.push_back({x, y});
    }

    return out;
}

int64_t task(std::string_view input) {
    auto data = parse(input);

    auto mult = data | std::views::transform(
        [](auto i) {
            return i.x * i.y;
    });

    auto sum = std::reduce(mult.begin(), mult.end());
    return sum;
}
