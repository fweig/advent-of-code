
#include "parse.h"
#include <cstdint>
#include <string_view>
#include <vector>
#include <ranges>
#include <unordered_set>

#define AOC_MAIN
#include "aoc.h"

void dump_nodes(grid<char> g, std::span<const vec2> nodes) {

    for (int y = 0; y < g.sheight(); y++) {
        for (int x = 0; x < g.swidth(); x++) {
            vec2 p(x, y);
            auto it = std::ranges::find(nodes, p);
            if (it == nodes.end()) {
                std::cout << ".";
            } else {
                std::cout << "X";
            }
        }
        std::cout << std::endl;
    }
}

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

std::pair<vec2, vec2> find_antinode_pair(vec2 a, vec2 b) {
    vec2 delta = b - a;
    return std::pair(b + delta, a - delta);
}

std::unordered_set<vec2> find_antinodes(grid<char> g) {

    std::unordered_set<vec2> antinodes;

    for (int y = 0; y < g.sheight(); y++) {
        for (int x = 0; x < g.swidth(); x++) {
            vec2 p(x, y);
            trace("{}, {}, {}, {}", x, y, g.width(), g.height());
            char node_type = g[p];

            if (node_type == '.') {
                continue;
            }

            for (int x2 = x + 1; x2 < g.swidth(); x2++) {
                vec2 p2(x2, y);
                if (g[p2] == node_type) {
                    auto [anode1, anode2] = find_antinode_pair(p, p2);
                    if (g.inside(anode1)) {
                        antinodes.insert(anode1);
                    }
                    if (g.inside(anode2)) {
                        antinodes.insert(anode2);
                    }
                }
            }

            for (int y2 = y + 1; y2 < g.sheight(); y2++) {
                for (int x2 = 0; x2 < g.swidth(); x2++) {
                    vec2 p2(x2, y2);
                    if (g[p2] == node_type) {
                        auto [anode1, anode2] = find_antinode_pair(p, p2);
                        if (g.inside(anode1)) {
                            antinodes.insert(anode1);
                        }
                        if (g.inside(anode2)) {
                            antinodes.insert(anode2);
                        }
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
    // trace("{}", nodes);
    // dump_nodes(g, nodes);
    return nodes.size();
}



