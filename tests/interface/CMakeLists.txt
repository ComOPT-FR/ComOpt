### CMakeLists.txt --- 
######################################################################
## 
### Commentary: 
## 
######################################################################

project(test_interface)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_SOURCES main.cc)

## #################################################################
## Build rules
## #################################################################

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} comopt ${comopt_DEPENDENCIES} ${comopt_DEPENDENCIES})

include(comoptTest)
