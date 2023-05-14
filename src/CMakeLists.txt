### CMakeLists.txt ---
##
######################################################################
##
### Commentary:
##
######################################################################

######################################################################
## Libraries
######################################################################

if(BUILD_SHARED_LIBS)
  set(LIBRARY_TYPE SHARED)
else()
  set(LIBRARY_TYPE STATIC)
endif()

foreach(DIR ${COMOPT_SOURCE_DIRECTORIES})
  add_subdirectory(${DIR})
endforeach()

######################################################################
## Build comopt library (shared or static)
######################################################################
unset(OBJECTS)
foreach(LIBRARY ${${PROJECT_NAME}_LIBRARIES})
  set(OBJECTS ${OBJECTS} $<TARGET_OBJECTS:${LIBRARY}>)
endforeach()

add_library(${PROJECT_NAME} ${LIBRARY_TYPE} ${OBJECTS})

target_link_libraries(model PUBLIC section)

# target_link_libraries(${PROJECT_NAME} ${comopt_DEPENDENCIES})

####################################################################
## Installation
####################################################################

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
