
cmake_minimum_required (VERSION 3.7)

project (forever C)

# include shared utilities
execute_process(COMMAND git clone https://github.com/sagiegurari/cmake-modules.git)
include("target/cmake-modules/src/utils.cmake")

set(CMAKE_BUILD_TYPE Release)
if(NOT WIN32)
  set(X_CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -Wcast-align -Wunused -Wshadow -Wpedantic")
endif()

set(X_CMAKE_PROJECT_ROOT_DIR ${CMAKE_BINARY_DIR}/..)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

# define all sources
file(GLOB SOURCES "src/*.c")
file(GLOB HEADER_SOURCES "include/*.h")
file(GLOB TEST_SOURCES "tests/*")
file(GLOB COMMON_TEST_SOURCES "tests/test.*")

# format code
if(DEFINED ENV{X_CMAKE_UNCRUSTIFY_ENABLED})
  add_custom_target(uncrustify ALL
    uncrustify
    -c ${X_CMAKE_PROJECT_ROOT_DIR}/uncrustify.cfg
    --no-backup ${SOURCES} ${HEADER_SOURCES} ${TEST_SOURCES})
endif()

# create static library
add_library(forever STATIC ${SOURCES})
if(NOT WIN32)
  set_target_properties(forever PROPERTIES COMPILE_FLAGS "${X_CMAKE_C_FLAGS} -Wconversion")
endif()

# tests
enable_testing()

macro(setup_test)
  utils_setup_c_test(${ARGV0} "${COMMON_TEST_SOURCES}" "${X_CMAKE_C_FLAGS}" "${CMAKE_BINARY_DIR}/bin")
endmacro(setup_test)

setup_test("options_single_retry")
setup_test("options_multiple_retries")
setup_test("options_with_delay")
setup_test("callback_multiple_retries")
