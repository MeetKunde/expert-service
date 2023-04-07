#!/bin/bash

accepted_build_types=("release" "debug")

if [ ! -z "$1" ]; then
    for acc in ${accepted_build_types[@]}; do
        clear
        if [[ $1 == $acc ]]; then
            ../out/build/unixlike-clang-${acc}/src/app/app
        fi
    done
else
    echo "Build type not provided!"
fi
