cmake_minimum_required(VERSION 3.18.0)
project(ses_lab)

include(FetchContent)
find_package(PkgConfig REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(BUILD_SERVER OFF)
option(BUILD_CONTROLLER ON)
option(BUILD_EXAMPLES OFF)

include(cjson)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(REPO_ROOT ${CMAKE_CURRENT_LIST_DIR})

if(BUILD_SERVER)
add_subdirectory(server)
endif()

if(BUILD_CONTROLLER)
add_subdirectory(raspi_controller)
endif()

if(BUILD_EXAMPLES)
add_subdirectory(examples)
endif()
