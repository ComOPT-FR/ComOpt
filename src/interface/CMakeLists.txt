### CMakeLists.txt ---
##
######################################################################
##
### Commentary:
##
######################################################################

project(interface)

######################################################################
## Sources
######################################################################

# Sources
file(GLOB ${PROJECT_NAME}_SOURCES [a-zA-Z0-9]*.cc)

# Include
file(GLOB ${PROJECT_NAME}_HEADERS [a-zA-Z0-9]*.h)

####################################################################
## Build rules
####################################################################

add_library(${PROJECT_NAME} OBJECT
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1)

if(NOT "${comopt_DEPENDENCIES_3RDPARTY}" STREQUAL "")
  add_dependencies(${PROJECT_NAME} ${comopt_DEPENDENCIES_3RDPARTY})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER src)

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME}
  EXPORT_FILE_NAME "${PROJECT_NAME}Export.h")

configure_file("${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}/include/." COPYONLY)

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS}
 "${CMAKE_BINARY_DIR}/include/${PROJECT_NAME}Export.h")

####################################################################
## Source file layout in development environments like Visual Studio
####################################################################

source_group("Header/Source Files" FILES ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})

####################################################################
## Installation
####################################################################

foreach(header ${${PROJECT_NAME}_HEADERS})
  string(REGEX REPLACE "(.*)\\.h\$" "\\1" h ${header})
endforeach(header)

install(FILES ${${PROJECT_NAME}_HEADERS} DESTINATION include/${PROJECT_NAME})

