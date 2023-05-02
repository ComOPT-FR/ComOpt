### comoptDependencies.cmake ---
##
######################################################################
##
### Commentary:
##
######################################################################

set(comopt_DEPENDENCIES)

# add eigen dependency from vcpkg
find_package(Eigen3 REQUIRED)
message(STATUS "Eigen3 found: ${EIGEN3_INCLUDE_DIR}")
include_directories(${EIGEN3_INCLUDE_DIR})

# add boost dependency from vcpkg
find_package(Boost REQUIRED)
message(STATUS "Boost found: ${Boost_INCLUDE_DIRS}")
include_directories(${Boost_INCLUDE_DIRS})


