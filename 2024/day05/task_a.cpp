
#include "parse.h"
#include <numeric>
#include <ranges>
#include <array>
#include <bitset>
#include <cassert>
#include <string_view>
#include <vector>

#define AOC_MAIN
#include "aoc.h"

inline constexpr size_t MAX_N = 100;

using manual_t = std::vector<int>;

class ruleset {

public:
    bool required_before(int x, int y) const {
        return m_required_before.at(x).test(y);
    }

    // x must appear BEFORE y
    void add_rule(int x, int y) {
        m_required_before.at(x).set(y);
    }

private:
    std::array<std::bitset<MAX_N>, MAX_N> m_required_before;
};

std::pair<ruleset, std::vector<manual_t>> parse(std::string_view input) {
    set_input(input);

    ruleset rules;

    for (;;) {
        // New line seperating rules and manuals
        if (try_consume_newline()) {
            break;
        }

        auto x = consume_int();
        consume("|");
        auto y = consume_int();
        consume("\n");

        trace("rule: {}, {}", x, y);
        rules.add_rule(x, y);
    }

    std::vector<manual_t> manuals;

    while (!at_end()) {
        auto &manual = manuals.emplace_back();
        while (!try_consume_newline()) {
            int x = consume_int();
            trace("{}", x);
            manual.push_back(x);

            try_consume(",");
        }
    }

    return {rules, manuals};
}

bool manual_valid(const ruleset &rules, const manual_t &manual) {
    for (size_t i = 0; i < manual.size(); i++) {
        for (size_t j = 0; j < i; j++) {
            int x = manual[i];
            int y = manual[j];

            if (rules.required_before(x, y)) {
                trace("{}|{} violated", x, y);
                return false;
            }
        }
    }

    return true;
}

int64_t middle_entry(const manual_t &manual) {
    auto x = manual.at(manual.size() / 2);
    trace("middle = {}", x);
    return x;
}

int64_t task(std::string_view input) {
    auto [rules, manuals] = parse(input);

    auto is_valid = [&](const manual_t &m) {
        return manual_valid(rules, m);
    };

    auto middle_entries = manuals
                       | std::views::filter(is_valid)
                       | std::views::transform(middle_entry);

    auto result = std::reduce(middle_entries.begin(), middle_entries.end());
    return result;
}
