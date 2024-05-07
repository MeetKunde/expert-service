cmake_minimum_required(VERSION 3.5)
project(app)

find_package(httplib REQUIRED PATHS ${CMAKE_BINARY_DIR})

include_directories(${EXPERT_HEADERS_DIR})

file(GLOB_RECURSE _SRCS "*.cpp" "*.h")

add_executable(app ${_SRCS})

target_link_libraries(app httplib::httplib expert)

install(TARGETS app DESTINATION bin)
