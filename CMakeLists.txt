cmake_minimum_required(VERSION 3.18.0)
project(ses_lab)

include(FetchContent)
find_package(PkgConfig REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(cjson)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(REPO_ROOT ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(server)
add_subdirectory(examples)
