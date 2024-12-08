#include "parse.h"
#include <cassert>
#include <string_view>
#include <vector>

#define AOC_MAIN
#include "aoc.h"


struct vec2 {
    int x;
    int y;

    constexpr vec2() : x(0), y(0) {}
    constexpr vec2(int x, int y) : x(x), y(y) {}

    vec2 operator+(vec2 o) {
        return vec2(x + o.x, y + o.y);
    }

    vec2 rotate90deg() const {
        return vec2(-y, x);
    }
};

inline constexpr vec2 Up(0, -1);
inline constexpr vec2 Down(0, 1);
inline constexpr vec2 Left(-1, 0);
inline constexpr vec2 Right(1, 0);

enum class Cell {
    Empty,
    Visited,
    Obstacle,
};

class grid {

public:
    grid(std::vector<Cell> &&data, size_t width)
        : data(std::move(data))
        , m_width(width)
        , m_height(this->data.size() / width) {
    }

    Cell &operator[](vec2 p) {
        assert(inside(p));
        return data[p.y * m_width + p.x];
    }

    Cell at_or(vec2 p, Cell fallback=Cell::Empty) {
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
    std::vector<Cell> data;
    size_t m_width;
    size_t m_height;
};

std::pair<grid, vec2> parse(std::string_view input) {
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

int64_t trace_guard(grid g, vec2 guard_pos) {
    int64_t visited = 0;
    vec2 dir = Up;
    while (g.inside(guard_pos)) {

        trace("{}, {}", guard_pos.x, guard_pos.y);

        switch (g[guard_pos]) {
        case Cell::Visited: break;
        case Cell::Empty:
            visited++;
            g[guard_pos] = Cell::Visited;
            break;
        default:
            break;
        }

        auto next_position = guard_pos + dir;
        if (g.at_or(next_position) == Cell::Obstacle) {
            dir = dir.rotate90deg();
            next_position = guard_pos + dir;
        }
        guard_pos = next_position;
    }

    return visited;
}

int64_t task(std::string_view input) {
    auto [g, p] = parse(input);
    auto visited = trace_guard(g, p);
    return visited;
}
