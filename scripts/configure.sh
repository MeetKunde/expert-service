#!/bin/bash

build_types=()
accepted_build_types=("Release" "Debug")

for acc in ${accepted_build_types[@]}; do
    build_types+=($acc)
done

for build_type in ${build_types[@]}; do
    echo "Generating ${build_type} configuration..."
    rm -rf ../build/${build_type}
    conan install .. -s build_type=${build_type} --build=missing
done
