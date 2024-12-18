#include "parse.h"
#include <cstdint>
#include <string_view>
#include <unordered_map>
#define AOC_MAIN
#include "aoc.h"

using stones_t = std::unordered_map<int64_t, int64_t>;

stones_t parse(std::string_view input) {
    stones_t stones;

    set_input(input);
    while (!at_end()) {
        while (!try_consume_newline()) {
            stones[consume_int()] += 1;
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
        x *= 10ll;
    }
    return x;
}

void blink(stones_t &stones) {
    stones_t next;

    for (auto it = stones.begin(); it != stones.end(); it++) {
        if (it->first == 0) {
           next[1] += it->second;
        } else if (int digits = ndigits(it->first); digits % 2 == 0) {
            auto d = pow10(digits / 2);

            auto upper = it->first / d;
            auto lower = it->first % d;

            next[upper] += it->second;
            next[lower] += it->second;
        } else {
            next[it->first * 2024] += it->second;
        }
    }

    stones = std::move(next);
}

void blinkn(stones_t &stones, size_t n=25) {
    for (size_t i = 0; i < n; i++) {
        blink(stones);
    }
}

int64_t stone_num(stones_t &stones) {
    int64_t n = 0;
    for (auto &it : stones) {
        n += it.second;
    }
    return n;
}

int64_t task(std::string_view input) {
    auto stones = parse(input);
    blinkn(stones, 75);
    auto n = stone_num(stones);
    std::println("{}", n);
    return n;
}
