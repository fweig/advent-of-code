#!/bin/bash

usage() {
    echo "$0 <day> <part> [test | trace | asan]"
    exit 1
}

mk_next_day() {
    day=$1
    mkdir -p $day
    touch $day/task_a.cpp
    touch $day/test_a.txt
    touch $day/input_a.txt
    touch $day/task_b.cpp
    touch $day/test_b.txt
    touch $day/input_b.txt
    exit 0
}

[[ $# -eq 0 ]] && usage
[[ $# -eq 1 ]] && mk_next_day $1

day=$1
part=$2
shift 2
for arg in "$@"; do declare $arg='1'; done

src=$day/task_$part.cpp
exe=build/${day}_task_$part

compile=g++
compile="$compile -xc++ -std=c++23 -Icommon -Wall -Wextra -Werror --max-errors=5 -ggdb"
[[ -v asan ]] && compile="$compile -Og"
[[ -v asan ]] || compile="$compile -O2"
[[ -v trace ]] && compile="$compile -DTRACE"

# compile="$compile -o $exe common/main.cpp $src"
compile="$compile -o $exe $src"

mkdir -p build

echo $compile
$compile
[[ $? -ne 0 ]] && exit 1

[[ -v test ]] && exe="$exe $day/input_test.txt" \
              || exe="$exe $day/input.txt"

echo $exe
$exe
