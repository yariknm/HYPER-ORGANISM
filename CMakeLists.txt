cmake_minimum_required(VERSION 3.16)
project(HYPER_ORGANISM VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -funroll-loops -flto -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

option(ENABLE_MPI "Enable MPI distributed support" OFF)
option(ENABLE_ARM64 "Enable ARM64 SVE backend" OFF)
option(ENABLE_TESTS "Build tests" ON)

include(CTest)
enable_testing()

add_subdirectory(src)
if(ENABLE_TESTS)
    add_subdirectory(tests)
endif()