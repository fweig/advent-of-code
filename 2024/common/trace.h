#pragma once

#include <iostream>
#include <span>

#ifndef TRACE
#define trace(...) ((void)0)
#else
template<typename T>
void trace(std::span<const T> data) {
    std::cout << "[ ";
    for (const auto &x : data) {
        std::cout << x << " ";
    }
    std::cout << ']' << std::endl;
}

template<typename Container>
void trace(const Container &c) {
    trace(std::span(c));
}
#endif
