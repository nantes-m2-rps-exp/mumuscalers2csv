cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(scalers LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(GNUInstallDirs)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/stage/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/stage/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/stage/bin)

set(BUILD_SHARED_LIBS ON)

find_package(aliphysics-lite REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(fmt REQUIRED)
find_package(ROOT REQUIRED)

add_subdirectory(src)

