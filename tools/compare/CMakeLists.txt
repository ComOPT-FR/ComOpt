### CMakeLists.txt --- 
######################################################################
## 
### Commentary: 
## 
######################################################################

project(compare)

######################################################################
## Sources
######################################################################

# Sources
set(${PROJECT_NAME}_SOURCES main.cc compareExact.cc compareSum.cc)

# Include
set(${PROJECT_NAME}_HEADERS compare.h)

## #################################################################
## Build rules
## #################################################################

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})

target_link_libraries(${PROJECT_NAME} ${comopt_DEPENDENCIES})

if(NOT "${comopt_DEPENDENCIES_3RDPARTY}" STREQUAL "")
  add_dependencies(${PROJECT_NAME} ${comopt_DEPENDENCIES_3RDPARTY})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER tools)
