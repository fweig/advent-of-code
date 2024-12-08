
#include "parse.h"
#include <cmath>
#include <cstdint>
#include <numeric>
#include <ranges>
#include <string_view>
#include <vector>
#define AOC_MAIN
#include "aoc.h"

struct Equation {
    int64_t result;
    std::vector<int64_t> operands;
};

int64_t concat(int64_t x, int64_t y) {
    int64_t shift = std::ceil(std::log10(y));
    return x * std::pow(10, shift) + y;
}

std::vector<Equation> parse(std::string_view input) {

    std::vector<Equation> equations;

    set_input(input);

    while (!at_end()) {
        auto &equation = equations.emplace_back();
        equation.result = consume_int();
        consume(":");
        while (!try_consume_newline()) {
            auto x = consume_int();
            equation.operands.push_back(x);
        }
    }

    return equations;
}


bool solvable(int64_t result, std::span<const int64_t> operands, int64_t fold = 0l) {
    if (operands.empty()) {
        return result == fold;
    }

    if (fold > result) {
        return false;
    }

    auto head = operands[0];
    auto tail = operands.subspan(1);

    if (fold == 0l) {
        return solvable(result, tail, head);
    }

    auto fold2 = fold + head;

    auto solve_add = solvable(result, tail, fold2);
    if (solve_add) {
        return true;
    }

    fold2 = fold * head;
    auto solve_mult = solvable(result, tail, fold2);
    return solve_mult;
}


bool solvable(const Equation &e) {
    return solvable(e.result, e.operands);
}


int64_t task(std::string_view input) {
    auto equations = parse(input);

    auto results =
        equations
        | std::views::filter([](auto &e) { return solvable(e); })
        | std::views::transform([](auto &e) { return e.result; });

    auto sum = std::reduce(results.begin(), results.end(), 0l);
    return sum;
}
