cmake_minimum_required(VERSION 3.5)
project(expertBackground CXX)

file(GLOB_RECURSE _HDRS "*.h")
file(GLOB_RECURSE _SRCS "*.cpp" "*.h")

find_package(nlohmann_json REQUIRED PATHS ${CMAKE_BINARY_DIR})

add_library(expertBackground SHARED STATIC ${_SRCS})
target_link_libraries(expertBackground symbolicAlgebra nlohmann_json::nlohmann_json)

install(TARGETS expertBackground DESTINATION ${EXPERT_BACKGROUND_INSTALL_LIB_DIR})
install(FILES ${_HDRS} DESTINATION ${EXPERT_BACKGROUND_INSTALL_INCLUDE_DIR})