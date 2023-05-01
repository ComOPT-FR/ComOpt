### CMakeLists.txt --- 
######################################################################
## 
### Commentary: 
## 
######################################################################

list_subdirectories_containing(${CMAKE_CURRENT_SOURCE_DIR} DIRS CMakeLists.txt)

if(WIN32)
  set(LIST_OF_TESTS "" CACHE STRING "List of available tests")
endif()

foreach(DIR ${DIRS})
  add_subdirectory(${DIR})
endforeach()

# Specific target that launches tests in a verbose mode (windows)
if(WIN32)
  add_custom_target(RUN_ALL_TESTS
    ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1
    ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --verbose --timeout 900
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

  set_target_properties(RUN_ALL_TESTS PROPERTIES FOLDER tests)

  set(TEST "" CACHE STRING "Create target for a specific test")

  set_property(CACHE TEST PROPERTY STRINGS ${LIST_OF_TESTS})
  mark_as_advanced(LIST_OF_TESTS)

  if(NOT "${TEST}" STREQUAL "")
    add_custom_target(RUN_${TEST}
      ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1
      ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --verbose -R ${TEST} --timeout 900
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

    set_target_properties(RUN_${TEST} PROPERTIES FOLDER tests)
  endif()
endif()
