#!/bin/bash

build_types=()
accepted_build_types=("Release" "Debug")

for arg in "$@"; do
    for acc in ${accepted_build_types[@]}; do
        if [[ $arg == $acc ]]; then
            build_types+=($arg)
        fi
    done
done

for build_type in ${build_types[@]}; do
    echo "Testing in ${build_type} mode..."
    ( cd .. && ctest -C ${build_type} --preset test-unixlike-gcc-${build_type,,} --output-on-failure -r xml -d yes --order lex)
done
