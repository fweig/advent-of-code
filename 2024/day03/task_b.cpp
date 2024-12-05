
#include "parse.h"
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
    set_input(input);

    std::vector<multiply_instr> out;

    bool enabled = true;

    while (!at_end()) {
        if (try_consume("do()")) {
            enabled = true;
            continue;
        }

        if (try_consume("don't()")) {
            enabled = false;
            continue;
        }

        if (!try_consume("mul(")) {
            advance();
            continue;
        }

        auto [x, ok] = try_consume_int();
        if (!ok) {
            continue;
        }

        if (!try_consume(",")) {
            continue;
        }

        auto [y, ok2] = try_consume_int();
        if (!ok2) {
            continue;
        }

        if (!try_consume(")")) {
            continue;
        }

        if (enabled) {
            out.push_back({x, y});
        }
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
