cmake_minimum_required(VERSION 3.15)

project(SpaceInvaders VERSION 0.1.0
                 DESCRIPTION "Space Invaders on the TI TM4C123GXL MCU"
                 LANGUAGES C)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE c_std_17)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(ENABLE_TESTING "Enable Test Builds" ON)

if(ENABLE_TESTING)
  enable_testing()
  message("Building Tests")
  add_subdirectory(test)
endif()

add_subdirectory(src)