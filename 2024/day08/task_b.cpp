
#include "parse.h"
#include <cstdint>
#include <string_view>
#include <vector>
#include <unordered_set>

#define AOC_MAIN
#include "aoc.h"


grid<char> parse(std::string_view input) {

    set_input(input);

    size_t width = 0;

    std::vector<char> grid_flat;

    while (!at_end()) {

        size_t current_width = 0;
        while (!try_consume_newline()) {
            current_width++;
            char c = advance();
            grid_flat.push_back(c);
        }

        width = std::max(width, current_width);
    }

    trace("{}", grid_flat);

    return grid(std::move(grid_flat), width);
}

void find_all_antinodes(vec2 a, vec2 b, const grid<char> &g, std::unordered_set<vec2> &nodes) {
    vec2 delta = b - a;

    for (vec2 antib = b; g.inside(antib); antib = antib + delta) {
        nodes.insert(antib);
    }

    for (vec2 antia = a; g.inside(antia); antia = antia - delta) {
        nodes.insert(antia);
    }

}

std::unordered_set<vec2> find_antinodes(grid<char> g) {

    std::unordered_set<vec2> antinodes;

    for (int y = 0; y < g.sheight(); y++) {
        for (int x = 0; x < g.swidth(); x++) {
            vec2 p(x, y);
            char node_type = g[p];

            if (node_type == '.') {
                continue;
            }

            for (int x2 = x + 1; x2 < g.swidth(); x2++) {
                vec2 p2(x2, y);
                if (g[p2] == node_type) {
                    find_all_antinodes(p, p2, g, antinodes);
                }
            }

            for (int y2 = y + 1; y2 < g.sheight(); y2++) {
                for (int x2 = 0; x2 < g.swidth(); x2++) {
                    vec2 p2(x2, y2);
                    if (g[p2] == node_type) {
                        find_all_antinodes(p, p2, g, antinodes);
                    }
                }
            }
        }
    }

    return antinodes;
}

int64_t task(std::string_view input) {
    auto g = parse(input);
    auto nodes = find_antinodes(g);
    return nodes.size();
}
