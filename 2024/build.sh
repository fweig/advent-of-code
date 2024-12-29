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

[[ "$(uname)" == "Linux" ]] && declare gcc=1
[[ "$(uname)" == "Darwin" ]] && declare clang=1

src=$day/task_$part.cpp
exe=build/${day}_task_$part

[[ -v gcc ]] && compile=g++-14
[[ -v clang ]] && compile=clang++
[[ -v gcc ]] && compile="$compile -xc++ -std=c++23 -Icommon -Wall -Wextra -Werror --max-errors=5 -g"
[[ -v clang ]] && compile="$compile -xc++ -std=c++23 -Icommon -Wall -Wextra -Werror -g"

[[ -v asan ]]  && compile="$compile -Og --sanitize=address"
[[ -v asan ]]  || compile="$compile -O2"
[[ -v trace ]] && compile="$compile -DTRACE"

compile="$compile -o $exe common/parse.cpp $src"

mkdir -p build

echo $compile
$compile
[[ $? -ne 0 ]] && exit 1

[[ -v test ]] && [[ -f $day/input_test_$part.txt ]] && exe="$exe $day/input_test_$part.txt"
[[ -v test ]] && [[ ! -f $day/input_test_$part.txt ]] && exe="$exe $day/input_test.txt"

[[ -v test0 ]] && exe="$exe $day/input0.txt"
[[ -v test1 ]] && exe="$exe $day/input1.txt"
[[ -v test2 ]] && exe="$exe $day/input2.txt"

[[ -v test || -v test0 || -v test1 || -v test2 ]] || exe="$exe $day/input.txt"

[[ -v gdb ]] && [[ "$(uname)" == "Linux" ]] && exe="gdb -batch -ex=r -ex=bt --args $exe"
[[ -v gdb ]] && [[ "$(uname)" == "Darwin" ]] && exe="lldb --batch -o "run" -o "bt" $exe"

echo $exe
$exe
