cmake_minimum_required(VERSION 3.5)
project(expertBackgroundTests)

include_directories(${EXPERT_BACKGROUND_HEADERS_DIR})

find_package(Catch2 REQUIRED PATHS ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE _SRCS "*.cpp" "*.h")

add_executable(expertBackgroundTests ${_SRCS})

target_link_libraries(expertBackgroundTests expertBackground Catch2::Catch2)

install(TARGETS expertBackgroundTests DESTINATION bin)

