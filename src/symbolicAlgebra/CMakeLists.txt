cmake_minimum_required(VERSION 3.5)
project(symbolicAlgebra CXX)

file(GLOB_RECURSE _HDRS "*.h")
file(GLOB_RECURSE _SRCS "*.cpp" "*.h")

add_library(symbolicAlgebra SHARED STATIC ${_SRCS})

install(TARGETS symbolicAlgebra DESTINATION ${ALGEBRA_INSTALL_LIB_DIR})
install(FILES ${_HDRS} DESTINATION ${ALGEBRA_INSTALL_INCLUDE_DIR})
