cmake_minimum_required(VERSION 3.5)
project(expert CXX)

file(GLOB_RECURSE _HDRS "*.h")
file(GLOB_RECURSE _SRCS "*.cpp" "*.h")

add_library(expert SHARED STATIC ${_SRCS} Tools.cpp theorems/AnglesTheorems.cpp theorems/LinesTheorems.cpp theorems/PolygonsTheorems.cpp)
target_link_libraries(expert expertBackground)

install(TARGETS expert DESTINATION ${EXPERT_INSTALL_LIB_DIR})
install(FILES ${_HDRS} DESTINATION ${EXPERT_INSTALL_INCLUDE_DIR})