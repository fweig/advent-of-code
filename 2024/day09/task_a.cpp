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
struct std::formatter<block> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const block& b, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{{{}, {}}}", b.idx, b.size);
    }
};


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

        if (empty) {
            blocks.push_back({-1, size});
        } else {
            blocks.push_back({index, size});
            index++;
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


void defragment(std::list<block> &blocks) {
    auto st = blocks.begin();
    st++;
    assert(st->empty());

    auto end = blocks.end();
    end--;
    assert(!end->empty());

    trace("{}" , blocks);

    while (std::distance(blocks.begin(), st) < std::distance(blocks.begin(), end)) {

        int size_empty = st->size;
        int size_move = end->size;

        block new_block = {end->idx, std::min(size_move, size_empty)};
        blocks.emplace(st, new_block);

        update_block_size(blocks, st, size_empty - size_move, true);
        update_block_size(blocks, end, size_move - size_empty, false);
        trace("{}" , blocks);
    }
}


int64_t checksum(std::list<block> &blocks) {
    int64_t i = 0, chk = 0;
    for (auto &b : blocks) {
        if (b.empty()) continue;
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
