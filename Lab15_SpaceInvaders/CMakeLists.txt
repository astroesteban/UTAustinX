cmake_minimum_required(VERSION 3.15)

project(SpaceInvaders VERSION 0.1.0
                 DESCRIPTION "Space Invaders on the TI TM4C123G MCU"
                 LANGUAGES C)

# Include the TM4C123 toolchain file
include(cmake/toolchains/tm4c123g/tm4c123g.cmake)

include(cmake/PreventInSourceBuilds.cmake)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(ENABLE_TESTING "Enable Test Builds" ON)

if(ENABLE_TESTING)
  enable_testing()
  message("Building Tests")
  add_subdirectory(test)
endif()

add_subdirectory(lib)
add_subdirectory(src)