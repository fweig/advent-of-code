#pragma once

#include <cstdlib>
#include <iterator>
#include <string_view>
#include <fstream>
#include <cstdio>
#include <iostream>

#include "trace.h"

int64_t task(std::string_view input);

bool consume(std::string_view &input, std::string_view prefix) {
    if (input.starts_with(prefix)) {
        input.remove_prefix(prefix.size());
        return true;
    }
    return false;
}

std::pair<int, bool> consume_int(std::string_view &input) {
    char *end;
    const int x = std::strtol(input.data(), &end, 10);
    ptrdiff_t bytes_read = end - input.data();

    input.remove_prefix(bytes_read);

    return {x, bytes_read > 0};
}

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
