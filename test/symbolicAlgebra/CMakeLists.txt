cmake_minimum_required(VERSION 3.5)
project(symbolicAlgebraTests)

include_directories(${ALGEBRA_HEADERS_DIR})

find_package(Catch2 REQUIRED PATHS ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE _SRCS "*.cpp" "*.h")

add_executable(symbolicAlgebraTests ${_SRCS})

target_link_libraries(symbolicAlgebraTests symbolicAlgebra Catch2::Catch2WithMain)

add_test(NAME symbolicAlgebraTests COMMAND symbolicAlgebraTests)

install(TARGETS symbolicAlgebraTests DESTINATION bin)

