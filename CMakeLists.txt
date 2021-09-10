cmake_minimum_required(VERSION 3.11)

set(VERSION_MAJOR 0 CACHE STRING "Project major version number")
set(VERSION_MINOR "1" CACHE STRING "Project minor version number")
set(VERSION_PATCH "0" CACHE STRING "Project patch version number")
set(NANOCBOR_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

project("nanocbor" LANGUAGES C CXX VERSION "${NANOCBOR_VERSION}")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Set the output of the libraries and executables.
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

###############################################################################
# OPTIONS
###############################################################################

option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(NANOCBOR_BUILD_TESTS "Build test suite" ON)
option(NANOCBOR_CLANG_TIDY "Build with clang-tidy" OFF)

if(BUILD_SHARED_LIBS)
  set(BUILD_STATIC_LIBS OFF)
else()
  set(BUILD_STATIC_LIBS ON)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

if(NANOCBOR_CLANG_TIDY)
  set(CMAKE_C_CLANG_TIDY clang-tidy; -checks=*;)
endif()

###############################################################################
# SOURCE FILES
###############################################################################

add_subdirectory(src)

###############################################################################
# ADD TESTS
###############################################################################

###############################################################################
# PRINT CONFIG
###############################################################################

message(STATUS "CMAKE_C_COMPILER:..................${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER:................${CMAKE_CXX_COMPILER}")
message(STATUS "NANOCBOR_BUILD_TYPE:...............${CMAKE_BUILD_TYPE}")
message(STATUS "NANOCBOR_BUILD_TESTS:..............${NANOCBOR_BUILD_TESTS}")
message(STATUS "NANOCBOR_SHARED_LIBS:..............${BUILD_SHARED_LIBS}")
