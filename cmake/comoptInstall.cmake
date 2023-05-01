### comoptInstall.cmake --- 
## 
######################################################################
## 
### Commentary: 
## 
######################################################################

set(LIBRARY_INSTALL_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY})
set(RUNTIME_INSTALL_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY})

set(${PROJECT_NAME}_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(${PROJECT_NAME}_INSTALL_LIBRARY_DIR ${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY})
set(${PROJECT_NAME}_INSTALL_RUNTIME_DIR ${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY})
set(${PROJECT_NAME}_INSTALL_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/cmake)
set(${PROJECT_NAME}_INSTALL_USE_FILE ${CMAKE_INSTALL_PREFIX}/cmake/${PROJECT_NAME}Use.cmake)

## #################################################################
## Setup configration files
## #################################################################

configure_file( ## Install tree configure file
  ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
  ${PROJECT_BINARY_DIR}/install/${PROJECT_NAME}Config.cmake
  @ONLY IMMEDIATE)

## #################################################################
## Setup use file
## #################################################################

configure_file( ## Common use file
  ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Use.cmake.in
  ${PROJECT_BINARY_DIR}/install/${PROJECT_NAME}Use.cmake
  @ONLY IMMEDIATE)

## #################################################################
## Path relocation
## #################################################################

if(APPLE)
  set(CMAKE_INSTALL_NAME_DIR "${LIBRARY_INSTALL_OUTPUT_PATH}")
endif()

if(UNIX AND NOT APPLE)
  set(CMAKE_INSTALL_RPATH "${LIBRARY_INSTALL_OUTPUT_PATH}")
endif()

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

## #################################################################
## Install cmake files
## #################################################################

install(FILES
  ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}CompilerFlags.cmake
  ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Utility.cmake
  ${PROJECT_BINARY_DIR}/install/${PROJECT_NAME}Use.cmake
  ${PROJECT_BINARY_DIR}/install/${PROJECT_NAME}Config.cmake
  DESTINATION
  cmake)

## #################################################################
## Installation
## #################################################################

# COMOPT INCLUDE FILES AND LIBRARIES
install_include("${comopt_INCLUDE_DIRS}" "*.h;*.tpp;*.inl")
copy_and_install_lib("${comopt_LIBRARY_DIRS}" "*.*")


