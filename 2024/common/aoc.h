#pragma once

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <string_view>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <format>
#include <vector>

#include "parse.h"
#include "trace.h"

struct vec2 {
    int x;
    int y;

    constexpr vec2() : x(0), y(0) {}
    constexpr vec2(int x, int y) : x(x), y(y) {}

    vec2 operator+(vec2 o) {
        return vec2(x + o.x, y + o.y);
    }

    bool operator==(vec2 o) const {
        return o.x == x && o.y == y;
    }

    vec2 operator-() {
        return vec2(-x, -y);
    }

    vec2 operator-(vec2 o) {
        return vec2(x - o.x, y - o.y);
    }

    vec2 rotate90deg() const {
        return vec2(-y, x);
    }
};

template<>
struct std::hash<vec2> {
    size_t operator()(vec2 v) const {
        return size_t(v.y) << 32 | size_t(v.x);
    }
};

template<>
struct std::formatter<vec2> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const vec2 &v, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({}, {})", v.x, v.y);
    }
};

inline constexpr vec2 Up(0, -1);
inline constexpr vec2 Down(0, 1);
inline constexpr vec2 Left(-1, 0);
inline constexpr vec2 Right(1, 0);

int direction_to_idx(vec2 d) {
    if (d == Up) return 0;
    if (d == Down) return 1;
    if (d == Left) return 2;
    if (d == Right) return 3;
    throw std::runtime_error("Invalid direction!");
}

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
    ptrdiff_t swidth() const { return m_width; }
    size_t height() const { return m_height; }
    ptrdiff_t sheight() const { return m_height; }

    bool inside(vec2 p) const {
        return 0 <= p.x && p.x < int(m_width) && 0 <= p.y && p.y < int(m_height);
    }

private:
    std::vector<T> data;
    size_t m_width;
    size_t m_height;
};

int64_t task(std::string_view input);

#ifdef AOC_MAIN
int main(int argc, const char** argv) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    const char *infile_name = argv[1];
    std::ifstream infile(infile_name);
    if (infile.fail()) {
        printf("Error: Couldn't open file '%s'\n", infile_name);
        return 1;
    }

    std::string input(std::istreambuf_iterator<char>(infile), {});

    auto result = task(input);
    printf("%ld\n", result);
    return 0;
}
#endif
