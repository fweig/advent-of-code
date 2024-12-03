#!/bin/bash

usage() {
    echo "$0 <day> <part> [test | trace | asan]"
    exit 1
}

mk_next_day() {
    day=$1
    mkdir -p $day
    touch $day/task_a.cpp
    touch $day/task_b.cpp
    touch $day/input_test.txt
    touch $day/input.txt
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

compile=g++-14
compile="$compile -xc++ -std=c++23 -Icommon -Wall -Wextra -Werror --max-errors=5 -ggdb"
[[ -v asan ]]  && compile="$compile -Og --sanitize=address"
[[ -v asan ]]  || compile="$compile -O2"
[[ -v trace ]] && compile="$compile -DTRACE"

compile="$compile -o $exe $src"

mkdir -p build

echo $compile
$compile
[[ $? -ne 0 ]] && exit 1

[[ -v test ]] && [[ -f $day/input_test_$part.txt ]] && exe="$exe $day/input_test_$part.txt"
[[ -v test ]] && [[ ! -f $day/input_test_$part.txt ]] && exe="$exe $day/input_test.txt"

[[ -v test ]] || exe="$exe $day/input.txt"

echo $exe
$exe
