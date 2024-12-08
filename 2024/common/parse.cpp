#include "parse.h"

#include <stdexcept>
#include <string_view>

static std::string_view I;

void set_input(std::string_view input) {
    I = input;
}

bool at_end() {
    return I.empty();
}

void advance(size_t n) {
    I.remove_prefix(n);
}

bool try_consume(std::string_view prefix) {
    if (I.starts_with(prefix)) {
        advance(prefix.size());
        return true;
    }
    return false;
}

void consume(std::string_view prefix) {
    bool ok = try_consume(prefix);
    if (!ok) {
        throw std::runtime_error("Failed to consume prefix!");
    }
}

bool try_consume_newline() {
    return try_consume("\n");
}

void consume_newline() {
    return consume("\n");
}

std::pair<int64_t, bool> try_consume_int() {
    char *end;
    const long x = std::strtol(I.data(), &end, 10);
    ptrdiff_t bytes_read = end - I.data();

    advance(bytes_read);

    return {x, bytes_read > 0};
}

int64_t consume_int() {
    auto [x, ok] = try_consume_int();

    if (!ok) {
        throw std::runtime_error("Failed to read int!");
    }
    return x;
}
