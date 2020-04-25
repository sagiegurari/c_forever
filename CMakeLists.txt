
cmake_minimum_required (VERSION 3.7)

project (forever C)

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
file(GLOB HEADER_SORUCES "include/*.h")
file(GLOB TEST_SOURCES "tests/*")
file(GLOB COMMON_TEST_SOURCES "tests/test.*")

# format code
if(DEFINED ENV{X_CMAKE_UNCRUSTIFY_ENABLED})
  add_custom_target(uncrustify ALL
    uncrustify
    -c ${X_CMAKE_PROJECT_ROOT_DIR}/uncrustify.cfg
    --no-backup ${SOURCES} ${HEADER_SORUCES} ${TEST_SOURCES})
endif()

# create static library
add_library(forever STATIC ${SOURCES})
if(NOT WIN32)
  set_target_properties(forever PROPERTIES COMPILE_FLAGS "${X_CMAKE_C_FLAGS} -Wconversion")
endif()

# tests
enable_testing()

macro(setup_test)
  message("Adding Test: ${ARGV0}")
  add_executable(test_${ARGV0} tests/test_${ARGV0}.c ${COMMON_TEST_SOURCES})
  target_link_libraries(test_${ARGV0} forever)
  set_target_properties(test_${ARGV0} PROPERTIES COMPILE_FLAGS "${X_CMAKE_C_FLAGS}")
  add_test(NAME ${ARGV0}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_${ARGV0})
endmacro(setup_test)

setup_test("options_single_retry")
setup_test("options_multiple_retries")
setup_test("options_with_delay")
setup_test("callback_multiple_retries")
