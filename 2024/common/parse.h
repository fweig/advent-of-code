#pragma once

#include <cstdint>
#include <string_view>

void set_input(std::string_view);

bool at_end();

char advance();
void advance(int n);


bool try_consume(std::string_view prefix);
void consume(std::string_view prefix);

bool try_consume_newline();
void consume_newline();

std::pair<int64_t, bool> try_consume_int();
int64_t consume_int();
