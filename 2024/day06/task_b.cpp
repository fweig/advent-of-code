#include "parse.h"
#include <bitset>
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <vector>

#define AOC_MAIN
#include "aoc.h"

enum class Cell {
    Empty,
    Visited,
    Obstacle,
};

template<typename  T>
class grid {

public:
    grid(std::vector<T> &&data, size_t width)
        : data(std::move(data))
        , m_width(width)
        , m_height(this->data.size() / width) {
    }

    T &operator[](vec2 p) {
        assert(inside(p));
        return data[p.y * m_width + p.x];
    }

    T at_or(vec2 p, T fallback) {
        if (inside(p)) {
            return (*this)[p];
        }
        return fallback;
    }

    size_t width() const { return m_width; }
    size_t height() const { return m_height; }

    bool inside(vec2 p) const {
        return 0 <= p.x && p.x < int(m_width) && 0 <= p.y && p.y < int(m_height);
    }

private:
    std::vector<T> data;
    size_t m_width;
    size_t m_height;
};


std::pair<grid<Cell>, vec2> parse(std::string_view input) {
    std::vector<Cell> grid_flat;

    size_t width = 0;

    vec2 guard_position;

    int x = 0, y = 0;

    set_input(input);
    while (!at_end()) {
        x = 0;
        while (!try_consume_newline()) {
            if (try_consume(".")) {
                grid_flat.push_back(Cell::Empty);
                x++;
                continue;
            }
            if (try_consume("#")) {
                grid_flat.push_back(Cell::Obstacle);
                x++;
                continue;
            }
            if (try_consume("^")) {
                guard_position = vec2(x, y);
                grid_flat.push_back(Cell::Empty);
                continue;
            }
        }
        y++;
        width = std::max<size_t>(width, x);
    }

    trace("width = {}, height = {}, total = {}", width, grid_flat.size() / width, grid_flat.size());

    return std::pair(grid(std::move(grid_flat), width), guard_position);
}


bool trace_guard(grid<Cell> g, grid<std::bitset<4>> visited, vec2 guard_pos, vec2 dir) {

    while (g.inside(guard_pos)) {

        trace("{}, {}", guard_pos.x, guard_pos.y);

        auto dir_idx = direction_to_idx(dir);

        if (g[guard_pos] == Cell::Visited && visited[guard_pos][dir_idx]) {
            return true; // found a cycle
        }

        switch (g[guard_pos]) {
        case Cell::Visited:
        case Cell::Empty:
            g[guard_pos] = Cell::Visited;
            visited[guard_pos][dir_idx] = true;
            break;
        default:
            break;
        }

        auto next_position = guard_pos + dir;
        if (g.at_or(next_position, Cell::Empty) == Cell::Obstacle) {
            dir = dir.rotate90deg();
        } else {
            guard_pos = next_position;
        }
    }

    return false; // left the map -> no cycles
}

int64_t trace_guard_obstacles(grid<Cell> g, vec2 guard_pos) {

    std::vector<std::bitset<4>> visited_flat(g.width() * g.height());
    grid<std::bitset<4>> visited(std::move(visited_flat), g.width());

    int64_t cycles = 0;

    for (int y = 0; y < int(g.height()); y++) {
        for (int x = 0; x < int(g.width()); x++) {
            vec2 p(x, y);

            if (p == guard_pos || g[p] != Cell::Empty) {
                continue;
            }

            g[p] = Cell::Obstacle;
            cycles += trace_guard(g, visited, guard_pos, Up);
            g[p] = Cell::Empty;
        }
    }

    return cycles;
}

int64_t task(std::string_view input) {
    auto [g, p] = parse(input);
    auto cycles = trace_guard_obstacles(g, p);
    return cycles;
}
