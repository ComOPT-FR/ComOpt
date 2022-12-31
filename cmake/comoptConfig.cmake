### comoptConfig.cmake --- 
## 
######################################################################
## 
### Commentary: Setup configration files
## 
######################################################################

## #################################################################
## Setup config file
## #################################################################

configure_file( ## Build tree configure file
  ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  @ONLY IMMEDIATE)

## #################################################################
## Setup use file
## #################################################################

configure_file( ## Common use file
  ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Use.cmake.in
  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Use.cmake
  @ONLY IMMEDIATE)

## #################################################################
## Install cmake files
## #################################################################

if(EXISTS ${${PROJECT_NAME}_SOURCE_DIR}/cmake/${PROJECT_NAME}Dependencies.cmake
    AND EXISTS ${${PROJECT_NAME}_SOURCE_DIR}/cmake/${PROJECT_NAME}Build.cmake
    AND EXISTS ${${PROJECT_NAME}_SOURCE_DIR}/cmake/${PROJECT_NAME}CompilerFlags.cmake
    AND EXISTS ${${PROJECT_NAME}_SOURCE_DIR}/cmake/${PROJECT_NAME}Utility.cmake
    AND EXISTS ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Use.cmake
    AND EXISTS ${${PROJECT_NAME}_BINARY_DIR}/install/${PROJECT_NAME}Config.cmake)

  install(FILES
    ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Dependencies.cmake
    ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Build.cmake
    ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}CompilerFlags.cmake
    ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Utility.cmake
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Use.cmake
    ${PROJECT_BINARY_DIR}/install/${PROJECT_NAME}Config.cmake
    DESTINATION
    cmake)
endif()
