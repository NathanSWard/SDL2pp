cmake_minimum_required(VERSION 3.15)

#
# Project details
#

project(
  ${CMAKE_PROJECT_NAME}Tests
  LANGUAGES CXX
)

verbose_message("Adding tests under ${CMAKE_PROJECT_NAME}Tests...")

#
# Set the sources for the unit tests and add the executable(s)
#

set(test_sources
  src/test.cpp
)
add_executable(${PROJECT_NAME} ${test_sources})

#
# Set the compiler standard
#

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

#
# Setup code coverage if enabled
#

if (${CMAKE_PROJECT_NAME}_ENABLE_CODE_COVERAGE)
  target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(${CMAKE_PROJECT_NAME} PUBLIC -fprofile-arcs -ftest-coverage)
  verbose_message("Code coverage is enabled and provided with GCC.")
endif()

#
# Load the desired unit testing framework
#

# TODO: Add Catch2 Testing!!!!
#find_package(Catch2 REQUIRED)
#target_link_libraries(
#  ${PROJECT_NAME} 
#  PUBLIC
#    Catch2::Catch2
#    Project
#)

#
# Add the unit tests
#

add_test(
  NAME
    ${PROJECT_NAME}
  COMMAND
    ${PROJECT_NAME}
)

verbose_message("Finished adding unit tests for ${CMAKE_PROJECT_NAME}.")
