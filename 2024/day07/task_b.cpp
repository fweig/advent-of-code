
#include "parse.h"
#include <cmath>
#include <cstdint>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <string_view>
#include <vector>
#define AOC_MAIN
#include "aoc.h"

struct Equation {
    int64_t result;
    std::vector<int64_t> operands;
};

#define SAFE_ADD(x, y) ({ \
    int64_t _r = 0; \
    if (__builtin_saddl_overflow(x, y, &_r)) { \
        throw std::runtime_error("Overflow in Add"); \
    } \
    _r; \
})

#define SAFE_MUL(x, y) ({ \
    int64_t _r = 0; \
    if (__builtin_smull_overflow(x, y, &_r)) { \
        throw std::runtime_error("Overflow in Mul"); \
    } \
    _r; \
})

int64_t concat(int64_t a, int64_t b) {
    // Calculate number of digits in b
    int64_t digits = 0;
    int64_t temp = b;
    while (temp > 0) {
        digits++;
        temp /= 10;
    }
    return a * (int64_t)std::pow(10, digits) + b;
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

    auto fold2 = SAFE_ADD(fold, head);

    auto solve_add = solvable(result, tail, fold2);
    if (solve_add) {
        return true;
    }

    fold2 = SAFE_MUL(fold, head);
    auto solve_mult = solvable(result, tail, fold2);
    if (solve_mult) {
        return true;
    }

    fold2 = concat(fold, head);
    auto solve_concat = solvable(result, tail, fold2);
    return solve_concat;
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
