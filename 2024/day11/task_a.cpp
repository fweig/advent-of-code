#include "parse.h"
#include <cstdint>
#include <list>
#include <string_view>
#define AOC_MAIN
#include "aoc.h"

std::list<int64_t> parse(std::string_view input) {
    std::list<int64_t> stones;

    set_input(input);
    while (!at_end()) {
        while (!try_consume_newline()) {
            stones.emplace_back(consume_int());
        }
    }

    return stones;
}

int ndigits(int64_t x) {
    if (x == 0) {
        return 1;
    }

    int n = 0;
    while (x != 0) {
        x /= 10;
        n++;
    }

    return n;
}

int64_t pow10(int n) {
    int64_t x = 1;
    for (; n > 0; n--) {
        x *= 10;
    }
    return x;
}

void blink(std::list<int64_t> &stones) {
    for (auto it = stones.begin(); it != stones.end(); it++) {
        if (*it == 0) {
            *it = 1;
        } else if (int digits = ndigits(*it); digits % 2 == 0) {
            auto d = pow10(digits / 2);

            auto upper = *it / d;
            auto lower = *it % d;

            stones.insert(it, upper);
            *it = lower;
        } else {
            *it *= 2024;
        }
    }
}

void blinkn(std::list<int64_t> &stones, size_t n=25) {
    trace("{}", stones);
    for (size_t i = 0; i < n; i++) {
        blink(stones);
        if (i < 5) {
            trace("{}", stones);
        }
    }
}

int64_t task(std::string_view input) {
    auto stones = parse(input);
    blinkn(stones);
    return stones.size();
}