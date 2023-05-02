### CMakeLists.txt --- 
######################################################################
## 
### Commentary: 
## 
######################################################################

project(test_linear_algebra)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_SOURCES main.cc)

## #################################################################
## Build rules
## #################################################################

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} comopt ${mpcp_DEPENDENCIES} ${mpcp_DEPENDENCIES})

include(comoptTest)
