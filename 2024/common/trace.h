#pragma once

#include <iostream>
#include <vector>

#ifndef TRACE
#define trace(x) ((void)0)
#else
template<typename T>
void trace(const std::vector<T> &data) {
    std::cout << '[';
    for (const auto &x : data) {
        std::cout << x << ", ";
    }
    std::cout << ']' << std::endl;
}
#endif
