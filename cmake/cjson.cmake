pkg_check_modules(CJSON QUIET libcjson)

if(CJSON_FOUND)
  message(STATUS "Using system cjson via pkg-config")

  function(link_with_cjson TARGET)
    target_link_libraries(${TARGET} PRIVATE ${CJSON_LIBRARIES})
    target_include_directories(${TARGET} PRIVATE ${CJSON_INCLUDE_DIRS})
    target_link_options(${TARGET} PRIVATE ${CJSON_LDFLAGS})
  endfunction()

  return()
endif()

message(STATUS "Fetching cjson ${CJSON_VERSION}")
set(ENABLE_CJSON_TEST
    OFF
    CACHE INTERNAL "")
set(ENABLE_CJSON_UTILS
    OFF
    CACHE INTERNAL "")
set(ENABLE_TARGET_EXPORT
    OFF
    CACHE INTERNAL "")
set(ENABLE_CUSTOM_COMPILER_FLAGS
    OFF
    CACHE INTERNAL "")
set(ENABLE_VALGRIND
    OFF
    CACHE INTERNAL "")
set(ENABLE_SANITIZERS
    OFF
    CACHE INTERNAL "")
set(ENABLE_SAFE_STACK
    OFF
    CACHE INTERNAL "")
set(BUILD_SHARED_LIBS
    OFF
    CACHE INTERNAL "")
set(BUILD_SHARED_AND_STATIC_LIBS
    OFF
    CACHE INTERNAL "")
set(CMAKE_INSTALL_PREFIX
    OFF
    CACHE INTERNAL "")
set(ENABLE_LOCALES
    OFF
    CACHE INTERNAL "")
set(CJSON_OVERRIDE_BUILD_SHARED_LIBS
    OFF
    CACHE INTERNAL "")
set(ENABLE_CJSON_VERSION_SO
    OFF
    CACHE INTERNAL "")

set(ENABLE_CJSON_UNINSTALL
    OFF
    CACHE INTERNAL "")

FetchContent_Declare(
  cjson
  GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
  GIT_TAG ${CJSON_VERSION}
  GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(cjson)
message(STATUS "cjson source dir ${cJSON_SOURCE_DIR}")

function(link_with_cjson TARGET)
  target_link_libraries(${TARGET} PRIVATE cjson)
  target_include_directories(${TARGET} PRIVATE ${cjson_SOURCE_DIR})
endfunction()
