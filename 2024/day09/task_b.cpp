#include "parse.h"
#include <iterator>
#include <list>
#include <string_view>

#define AOC_MAIN
#include "aoc.h"


struct block {
    int idx;
    int size;

    bool empty() const { return idx < 0; }
};

template <>
struct std::formatter<std::list<block>> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const std::list<block>& blocks, std::format_context& ctx) const {

        for (const auto &b : blocks) {
            if (b.empty()) {
                for (int i = 0; i < b.size; i++) {
                    std::format_to(ctx.out(), ".");
                }
            } else {
                for (int i = 0; i < b.size; i++) {
                    std::format_to(ctx.out(), "{}", b.idx);
                }
            }
        }

        return ctx.out();
    }
};


using iterator_t = std::list<block>::iterator;


int ctoi(char c) {
    assert('0' <= c && c <= '9');
    return c - '0';
}


std::list<block> parse(std::string_view input) {

    std::list<block> blocks;

    set_input(input);

    bool empty = false;
    int index = 0;

    while (!try_consume_newline()) {
        int size = ctoi(advance());

        if (size > 0) {
            if (empty) {
                blocks.push_back({-1, size});
            } else {
                blocks.push_back({index, size});
                index++;
            }
        }
        empty = !empty;
    }
    assert(at_end());
    return blocks;
}


void update_block_size(std::list<block> &blocks, std::list<block>::iterator &it, int new_size, bool forward) {
    if (new_size > 0) {
        it->size = new_size;
        return;
    }

    auto orig = it;

    if (forward) {
        for (it++; !it->empty(); it++);
    } else {
        for (it--; it->empty(); it--);
    }

    blocks.erase(orig);
}


void merge_blocks(std::list<block> &blocks, iterator_t &st, iterator_t &end) {

    for (auto it = st; it != end; it++) {
        auto it_next = std::next(it);
        if (it_next->idx == it->idx) {
            if (it_next == st) {
                st = it;
            }
            if (it_next == end) {
                end = it;
            }
            it->size += it_next->size;
            blocks.erase(it_next);
        }
    }

}

iterator_t find_next_empty(std::list<block> &blocks, iterator_t st) {
    while (st != blocks.end() && !st->empty()) {
        st++;
    }
    return st;
}


void defragment(std::list<block> &blocks) {
    trace("{}" , blocks);

    for (auto end = --blocks.end(); end != blocks.begin(); end--) {

        if (end->empty()) continue;

        for (auto st = blocks.begin(); st != end; st++) {

            // trace("st = {}, {}", *st, std::distance(blocks.begin(), st));

            if (!st->empty()) continue;

            int size_empty = st->size;
            int size_move = end->size;

            if (size_empty < size_move) continue;

            block new_block = {end->idx, std::min(size_move, size_empty)};
            blocks.emplace(st, new_block);

            update_block_size(blocks, st, size_empty - size_move, true);

            end->idx = -1;
            merge_blocks(blocks, end, st);
            trace("{}" , blocks);
            break;
        }



    }
}


int64_t checksum(std::list<block> &blocks) {
    int64_t i = 0, chk = 0;
    for (auto &b : blocks) {
        if (b.empty()) {
            i += b.size;
            continue;
        }
        for (int x = 0; x < b.size; x++) {
            chk += i * b.idx;
            i++;
        }
    }
    return chk;
}


int64_t task(std::string_view input) {
    auto blocks = parse(input);

    defragment(blocks);
    auto chk = checksum(blocks);
    return chk;
}
