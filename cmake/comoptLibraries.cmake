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

set(${PROJECT_NAME}_LIBRARIES 
  ## Add here the list of required libraries 
  coreOptimization
  basics

)

######################################################################
## Optional libraries
######################################################################


######################################################################
## Configuration file (setting macros)
######################################################################

# configure_file(
#   ${CMAKE_SOURCE_DIR}/cmake/comoptConfig.h.in
#   ${CMAKE_BINARY_DIR}/include/comoptConfig.h)

# install(FILES ${CMAKE_BINARY_DIR}/include/comoptConfig.h DESTINATION include)
