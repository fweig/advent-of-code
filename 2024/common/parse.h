#pragma once

#include <string_view>

void set_input(std::string_view);

bool at_end();

void advance(size_t n=1);

bool try_consume(std::string_view prefix);
void consume(std::string_view prefix);

bool try_consume_newline();
void consume_newline();

std::pair<int, bool> try_consume_int();
int consume_int();
