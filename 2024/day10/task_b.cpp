#include "parse.h"
#include <algorithm>
#include <cstdint>
#include <unordered_set>
#define AOC_MAIN
#include "aoc.h"

struct path_cache {
    bool visited = false;
    int paths = 0;
};

grid<int8_t> parse(std::string_view input) {

    set_input(input);
    
    std::vector<int8_t> grid_flat;

    int width = 0;
    while (!at_end()) {
        int current_width = 0;
        while (!try_consume_newline()) {
            grid_flat.push_back(advance() - '0');
            current_width++;
        }
        width = std::max(current_width, width);
    }

    return grid(std::move(grid_flat), width);
}

int64_t walk_path(vec2 pos, grid<int8_t> &data, grid<path_cache> &cache) {
    auto &c = cache[pos];

    int height = data[pos];
    if (height == 9) {
        c.visited = true;
        c.paths = 1;
    }

    if (c.visited) {
        return  c.paths;
    }

    vec2 npos = pos + Up;
    if (data.at_or(npos, 0) == height + 1) {
        c.paths += walk_path(npos, data, cache);
    }

    npos = pos + Down;
    if (data.at_or(npos, 0) == height + 1) {
        c.paths += walk_path(npos, data, cache);
    }

    npos = pos + Left;
    if (data.at_or(npos, 0) == height + 1) {
        c.paths += walk_path(npos, data, cache);
    }

    npos = pos + Right;
    if (data.at_or(npos, 0) == height + 1) {
        c.paths += walk_path(npos, data, cache);
    }

    c.visited = true;
    return c.paths;
}

int64_t trailhead_score(grid<int8_t> &data) {
    grid<path_cache> cache(data.width(), data.height());

    int64_t score = 0;

    vec2 p;
    for (p.y = 0; p.y < data.sheight(); p.y++) {
        for (p.x = 0; p.x < data.swidth(); p.x++) {
            if (data[p] == 0) {
                score += walk_path(p, data, cache);
            }
        }
    }

    return score;
}

int64_t task(std::string_view input) {
    auto data = parse(input);
    auto score = trailhead_score(data);
    return score;
}