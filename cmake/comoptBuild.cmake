### comoptBuild.cmake --- 
## 
######################################################################
## 
### Commentary: 
## 
######################################################################

include(GenerateExportHeader)

## #################################################################
## Get hostname
## #################################################################

cmake_host_system_information(RESULT HOSTNAME QUERY HOSTNAME)

## #################################################################
## Configure arch
## #################################################################

if(MSVC)
  option(COMOPT_PRECOMPILED_HEADERS "Use precompiled headers" true)

  # Turn on the ability to create folders to organize projects (.vcproj)
  # It creates "CMakePredefinedTargets" folder by default and adds CMake
  # defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  set(CMAKE_SUPPRESS_REGENERATION TRUE)

  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif(MSVC)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

## #################################################################
## Libraries
## #################################################################

include(comoptLibraries)

## #################################################################
## Compiler flags
## #################################################################

include(comoptCompilerFlags)

## #################################################################
## Include directory
## #################################################################

list_subdirectories_with_sources(${PROJECT_SOURCE_DIR}/src "${${PROJECT_NAME}_LIBRARIES}" COMOPT_SOURCE_DIRECTORIES)
include_directories(${PROJECT_BINARY_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
if(BUILD_WRAPPERS)
  include_directories(${PROJECT_SOURCE_DIR}/wrapping/python)
endif()

if(NOT IS_DIRECTORY ${CMAKE_BINARY_DIR}/include)
  file(REMOVE ${CMAKE_BINARY_DIR}/include)
endif()

if(NOT EXISTS ${CMAKE_BINARY_DIR}/include)
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)
endif()

## #################################################################
## Install prefix
## #################################################################

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  if(WIN32)
    string(REPLACE "\\" "/" ProgramFiles "$ENV{ProgramFiles}")
    set(CMAKE_INSTALL_PREFIX "${ProgramFiles}/${PROJECT_NAME}" CACHE PATH "${PROJECT_NAME} install prefix" FORCE)
  else()
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install CACHE PATH "CMake install prefix" FORCE)
  endif()
endif()

## #################################################################
## Configure path
## #################################################################

set(${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY bin)
set(${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY lib)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY})

if(WIN32)
  foreach(CONFIG_TYPES ${CMAKE_CONFIGURATION_TYPES}) ## for multi config types (MSVC based)
    string(TOUPPER ${CONFIG_TYPES} CONFIG_TYPES_UC)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_TYPES_UC} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_TYPES_UC} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_TYPES_UC} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  endforeach()
endif()

set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src ${PROJECT_BINARY_DIR}/include) 
set(${PROJECT_NAME}_LIBRARY_DIRS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(${PROJECT_NAME}_RUNTIME_DIRS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(${PROJECT_NAME}_USE_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Use.cmake)

## #################################################################
## Path relocation
## #################################################################

set(LIBRARY_INSTALL_OUTPUT_PATH "$ORIGIN/../${${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY}")

if(APPLE)
  set(CMAKE_INSTALL_NAME_DIR "${LIBRARY_INSTALL_OUTPUT_PATH}")
endif ()

if(UNIX AND NOT APPLE)
  set(CMAKE_INSTALL_RPATH "${LIBRARY_INSTALL_OUTPUT_PATH}")
endif ()

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
