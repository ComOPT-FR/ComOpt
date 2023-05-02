### CMakeLists.txt ---
##
######################################################################
##
### Commentary:
##
######################################################################

cmake_minimum_required(VERSION 3.4.0)

###################
# configure vcpkg #

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

###################

# Main project name

project(comopt)

##################

set(CMAKE_CXX_STANDARD 17)

# Building choices

option(BUILD_SHARED_LIBS   "Build shared libraries"  false)
option(BUILD_TOOLS         "Build tools"             true)
option(BUILD_TESTS         "Build tests"             true)
option(BUILD_DOCUMENTATION "Build documentation"     false)

###################

# Load basic functions

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(comoptUtility)

##################

# Include necessaries routines and directories

include(comoptBuild)
include(comoptDependencies)

add_subdirectory(src)

include(comoptConfig)

if(BUILD_TOOLS)
  add_subdirectory(tools)
endif()

if(BUILD_DOCUMENTATION)
  add_subdirectory(doc)
endif()

if(BUILD_TESTS)
  if(BUILD_TOOLS)
    enable_testing()
    add_subdirectory(tests)
  else()
    message(FATAL_ERROR "\"BUILD_TESTS\" requires \"BUILD_TOOLS\"")
  endif()
endif()

##################

# Install

include(comoptInstall)

##################

# Hide options

markVariablesAsAdvanced()

