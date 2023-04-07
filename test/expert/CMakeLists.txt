cmake_minimum_required(VERSION 3.5)
project(expertTests)

include_directories(${EXPERT_HEADERS_DIR})

find_package(Catch2 REQUIRED PATHS ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE _SRCS "*.cpp" "*.h")

add_executable(expertTests ${_SRCS})

target_link_libraries(expertTests expert Catch2::Catch2)

install(TARGETS expertTests DESTINATION bin)

