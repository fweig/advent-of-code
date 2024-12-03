#define AOC_IMPL
#include "aoc.h"
#include <initializer_list>

enum Dir {
  Up,
  Down,
  Left,
  Right,
};

idx2 dir2delta(Dir d)
{
  constexpr idx2 delta[] = {
    { 0, -1 },
    { 0, 1 },
    { -1, 0 },
    { 1, 0 },
  };
  return delta[d];
}

void dump(iarr4d cache)
{
  foreach2d(cache) {
    i64 x = LONG_MAX;
    for (Dir d : {Up, Down, Left, Right}) {
      for (i64 l = 0; l < 3; l++) {
        x = min(x, cache(_i.x, _i.y, d, l));
      }
    }
    printf("%ld ", x);
    if (_i.x == width(cache) - 1)
      putc('\n', stdout);
  }
}

// arr<idx2> findpath(iarr4d cache) {

// }

i64 walk_(idx2 p, Dir d, i64 l, iarr2d in, iarr4d cache, carr2d visited)
{
  if (l >= 3)
    return LONG_MAX;

  if (!inside(p, in.dim))
    return LONG_MAX;

  if (visited(p))
    return LONG_MAX;

  if (p == idx2{ width(in) - 1, height(in) - 1 })
    return in(p);

  i64 *cachedval = &cache(p.x, p.y, d, l);
  if (*cachedval != LONG_MAX)
    return *cachedval;

  visited(p) = 1;

  i64 val = LONG_MAX;
  for (auto d0 : { Up, Down, Left, Right }) {
    val = min(val, walk_(p + dir2delta(d0), d0, d0 == d ? l + 1 : 0, in, cache,
                         visited));
  }

  visited(p) = 0;

  if (val == LONG_MAX)
    return LONG_MAX;

  req(*cachedval == LONG_MAX);

  *cachedval = val + in(p);
  return *cachedval;
}

i64 walk(idx2 p, iarr2d in)
{
  iarr4d cache(width(in), height(in), 4, 3);
  fill(cache, LONG_MAX);
  iarr dists;
  carr2d visited(in.dim);
  fill(visited, (char)0);
  for (auto d : { Up, Down, Left, Right })
    put(dists, walk_(p + dir2delta(d), d, 0, in, cache, visited));
  dump(cache);
  return min(dists);
}

i64 task1(char *infile)
{
  iarr2d in = map2d(to_arr2d(frl(infile)), i64, *it - '0');
  return walk({ 0, 0 }, in);
}

i64 task2(char *input) { return 0; }
