#include "parse.h"
#include <string_view>
#define AOC_MAIN
#include "aoc.h"

grid<char> parse(std::string_view input) {
    std::vector<char> data_flat;

    size_t width = 0;

    set_input(input);
    while (!at_end()) {

        size_t current_width = 0;
        while (!try_consume_newline()) {
            data_flat.push_back(advance());
            current_width++;
        }

        width = std::max(width, current_width);
    }

    return grid(std::move(data_flat), width);
}

void assign_region(vec2 p, int region, grid<char> &g, grid<int> &region_id) {
    auto c = g[p];

    assert(region > -1);
    region_id[p] = region;

    auto neighbor_up = g.at_or(p + Up, 0);
    auto region_up = region_id.at_or(p + Up, -1);

    auto neighbor_down = g.at_or(p + Down, 0);
    auto region_down = region_id.at_or(p + Down, -1);

    auto neighbor_left = g.at_or(p + Left, 0);
    auto region_left = region_id.at_or(p + Left, -1);

    auto neighbor_right = g.at_or(p + Right, 0);
    auto region_right = region_id.at_or(p + Right, -1);

    if (neighbor_up == c && region_up == -1) {
        assign_region(p + Up, region, g, region_id);
    }

    if (neighbor_down == c && region_down == -1) {
        assign_region(p + Down, region, g, region_id);
    }

    if (neighbor_left == c && region_left == -1) {
        assign_region(p + Left, region, g, region_id);
    }

    if (neighbor_right == c && region_right == -1) {
        assign_region(p + Right, region, g, region_id);
    }
}

int64_t total_fencing_price(grid<char> &g) {
    grid<int> region_id(g.width(), g.height());
    region_id.set_all(-1);

    struct region_info {
        size_t ncells = 0;
        size_t nfences = 0;
    };
    std::vector<region_info> regions;

    for (vec2 p; p.y < g.sheight(); p.y++) {
        for (p.x = 0; p.x < g.swidth(); p.x++) {

            auto c = g[p];

            auto neighbor_up = g.at_or(p + Up, 0);
            auto region_up = region_id.at_or(p + Up, -1);

            auto neighbor_down = g.at_or(p + Down, 0);
            auto region_down = region_id.at_or(p + Down, -1);

            auto neighbor_left = g.at_or(p + Left, 0);
            auto region_left = region_id.at_or(p + Left, -1);

            auto neighbor_right = g.at_or(p + Right, 0);
            auto region_right = region_id.at_or(p + Right, -1);

            int my_region = -1;
            if (c == neighbor_up && region_up > -1) {
                my_region = region_up;
            } else if (c == neighbor_down && region_down > -1) {
                my_region = region_down;
            } else if (c == neighbor_left && region_left > -1) {
                my_region = region_left;
            } else if (c == neighbor_right && region_right > -1) {
                my_region = region_right;
            } else {
                my_region = regions.size();
                regions.emplace_back();
                assign_region(p, my_region, g, region_id);
            }

            region_id[p] = my_region;

            auto &region_info = regions.at(my_region);

            region_info.ncells++;

            region_info.nfences += c != neighbor_down;
            region_info.nfences += c != neighbor_up;
            region_info.nfences += c != neighbor_left;
            region_info.nfences += c != neighbor_right;
        }
    }

    int64_t total_price = 0;
    for (auto &i : regions) {
        std::println("{} * {}", i.ncells, i.nfences);
        total_price += i.ncells * i.nfences;
    }

    return total_price;
}

int64_t task(std::string_view input) {
    auto g = parse(input);
    auto price = total_fencing_price(g);
    return price;
}
