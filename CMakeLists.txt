cmake_minimum_required(VERSION 3.15.0)

project(expert-service
        VERSION 1.0.0
        DESCRIPTION ""
        HOMEPAGE_URL "www.expert-service.com"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O3")

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(ALGEBRA_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(ALGEBRA_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(ALGEBRA_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

set(EXPERT_BACKGROUND_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(EXPERT_BACKGROUND_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(EXPERT_BACKGROUND_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

set(EXPERT_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(EXPERT_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(EXPERT_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

set(ALGEBRA_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/symbolicAlgebra)
set(EXPERT_BACKGROUND_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/expertBackground)
set(EXPERT_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/expert)

include_directories(${ALGEBRA_INSTALL_INCLUDE_DIR})
include_directories(${ALGEBRA_HEADERS_DIR})

include_directories(${EXPERT_BACKGROUND_INSTALL_INCLUDE_DIR})
include_directories(${EXPERT_BACKGROUND_HEADERS_DIR})

include_directories(${EXPERT_INSTALL_INCLUDE_DIR})
include_directories(${EXPERT_HEADERS_DIR})

set(ENABLE_DEVELOPER_MODE TRUE CACHE BOOL "Enable 'developer mode'")

set(OPT_WARNINGS_AS_ERRORS_DEVELOPER_DEFAULT TRUE)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif ()

#if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
#    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
#    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
#            "${CMAKE_BINARY_DIR}/conan.cmake"
#            TLS_VERIFY ON)
#endif()
#
#include(${CMAKE_BINARY_DIR}/conan.cmake)
#
#conan_cmake_autodetect(settings)
#
#conan_cmake_install(PATH_OR_REFERENCE ${PROJECT_SOURCE_DIR}
#        BUILD missing
#        SETTINGS ${settings})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH})

add_subdirectory(src)

option(ENABLE_TESTING "Enable the tests" ${PROJECT_IS_TOP_LEVEL})
if (ENABLE_TESTING)
    enable_testing()
    add_subdirectory(test)
endif ()

add_compile_options(-Wall -Wextra -Werror -pedantic -fprofile-instr-generate -fcoverage-mapping)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT expert-service)

if (CMAKE_SKIP_INSTALL_RULES)
    return()
elseif (NOT PROJECT_IS_TOP_LEVEL)
    return()
endif ()