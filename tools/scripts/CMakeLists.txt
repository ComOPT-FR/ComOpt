### CMakeLists.txt --- 
######################################################################
## 
### Commentary: 
## 
######################################################################

## #################################################################
## Copy mpcp_test script in binrary directory
## #################################################################

if(WIN32)
  set(EXTENSION bat)
else()
  set(EXTENSION sh)
endif()

set(TEST_SCRIPT comopt_test.${EXTENSION})

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_SCRIPT}
  ${EXECUTABLE_OUTPUT_PATH}/${TEST_SCRIPT}
  COPYONLY IMMEDIATE)
