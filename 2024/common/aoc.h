#pragma once

#include <cstdlib>
#include <iterator>
#include <string_view>
#include <fstream>
#include <cstdio>
#include <iostream>

#include "trace.h"

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
