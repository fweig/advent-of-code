
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

#define AOC_MAIN
#include "aoc.h"

class str2d {

public:
    str2d(std::vector<char> &&data, size_t width, size_t height)
        : data(std::move(data))
        , m_width(width)
        , m_height(height) {
    }

    char operator[](int x, int y, char out_of_bounds = '\0') const {
        if (x < 0 || int(m_width) <= x) {
            return out_of_bounds;
        }

        if (y < 0 || int(m_height) <= y) {
            return out_of_bounds;
        }

        return data[y * m_width + x];
    }

    size_t width() const { return m_width; }
    size_t height() const { return m_height; }

private:
    std::vector<char> data;
    size_t m_width;
    size_t m_height;
};

str2d parse(std::string_view input) {
    std::vector<char> input_flat;
    size_t width = 0, height = 0;

    size_t line_length = 0;
    for (auto c : input) {
        switch (c) {
        case '\n':
            height++;
            assert(width == 0 || line_length == width);
            width = line_length;
            line_length = 0;
            break;
        default:
            input_flat.push_back(c);
            line_length++;
        }
    }

    return str2d(std::move(input_flat), width, height);
}

int64_t count_string_single_direction(const str2d &input, int x, int y, int dx, int dy, std::string_view str) {
    for (auto c : str) {
        x += dx;
        y += dy;

        if (input[x, y] != c) {
            return 0;
        }
    }

    return 1;
}

int64_t count_string_all_directions(const str2d &input, int x, int y, std::string_view str) {
    int64_t c = 0;

    c += count_string_single_direction(input, x, y, -1, -1, str);
    c += count_string_single_direction(input, x, y, -1,  0, str);
    c += count_string_single_direction(input, x, y, -1,  1, str);
    c += count_string_single_direction(input, x, y,  0, -1, str);
    c += count_string_single_direction(input, x, y,  0,  1, str);
    c += count_string_single_direction(input, x, y,  1, -1, str);
    c += count_string_single_direction(input, x, y,  1,  0, str);
    c += count_string_single_direction(input, x, y,  1,  1, str);

    return c;
}

int64_t count_xmas(const str2d &input) {

    int64_t xmas_count = 0;

    for (size_t y = 0; y < input.height(); y++) {
        for (size_t x = 0; x < input.width(); x++) {
            switch (input[x, y]) {
            case 'X':
                xmas_count += count_string_all_directions(input, x, y, "MAS");
                break;
            // case 'S':
            //     xmas_count += count_string_all_directions(input, x, y, "AMX");
            //     break;
            }
        }
    }

    return xmas_count;
}

int64_t task(std::string_view input) {
    auto map = parse(input);
    auto n = count_xmas(map);
    return n;
}