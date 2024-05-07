#!/bin/bash

build_types=()
accepted_build_types=("Debug" "Release")

for arg in "$@"; do
    for acc in ${accepted_build_types[@]}; do
        if [[ $arg == $acc ]]; then
            build_types+=($arg)
        fi
    done
done

procs_number=$(nproc)


for build_type in ${build_types[@]}; do
    echo "Building in ${build_type,,} mode on $procs_number procs..."
    echo "After change gcc to clang or clang to gcc, you need to run configure.sh again to generate the build files."

    (cd .. && cmake --preset unixlike-clang-${build_type,,})
    (cd .. && cmake --build --preset build-unixlike-clang-${build_type,,} --parallel $procs_number)
done
