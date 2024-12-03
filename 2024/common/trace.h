#pragma once

#include <print>

#ifndef TRACE
#define trace(...) ((void)0)
#else

#define trace(fmt, ...) std::print(fmt "\n", ##__VA_ARGS__)

// Concepts to distinguish container types
template<typename T>
concept MapLike = requires(T t) {
    typename T::key_type;
    typename T::mapped_type;
    t.begin()->first;
    t.begin()->second;
};

template<typename T>
concept SequenceContainer = std::ranges::range<T> && !MapLike<T>;

// Formatter for sequence containers (vector, array, span, etc.)
template <SequenceContainer T>
struct std::formatter<T> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const T& container, std::format_context& ctx) const {
        if (std::ranges::empty(container)) {
            return std::format_to(ctx.out(), "[]");
        }

        auto it = std::ranges::begin(container);
        auto out = std::format_to(ctx.out(), "[");

        while (it != std::prev(std::ranges::end(container))) {
            std::format_to(out, "{}, ", *it);
            ++it;
        }
        return std::format_to(out, "{}]", *it);
    }
};

// Formatter for map-like containers
template <MapLike T>
struct std::formatter<T> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const T& map, std::format_context& ctx) const {
        if (map.empty()) {
            return std::format_to(ctx.out(), "{}");
        }

        auto out = std::format_to(ctx.out(), "{{");
        auto it = map.begin();
        while (it != std::prev(map.end())) {
            std::format_to(out, "{}: {}, ", it->first, it->second);
            ++it;
        }
        return std::format_to(out, "{}: {}}}", it->first, it->second);
    }
};

#endif
