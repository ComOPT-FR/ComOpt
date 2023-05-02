### comoptDependencies.cmake ---
##
######################################################################
##
### Commentary:
##
######################################################################

set(comopt_DEPENDENCIES)
set(comopt_DEPENDENCIES_3RDPARTY)

## #################################################################
## Third-party libraries configuration
## #################################################################

include(3rdParty)

set(3rdParty_DIR ${CMAKE_BINARY_DIR}/3rdParty)
  # Eigen dependency
  set(EIGEN_LIBRARY eigen)
  set(EIGEN_TARGET)
  unset(DIR)

  if(DEFINED ENV{Eigen3_DIR})
    find_package(Eigen3 3.3 QUIET NO_MODULE)

    if (TARGET Eigen3::Eigen)
      set(DIR $ENV{Eigen3_DIR})

      set(comopt_DEPENDENCIES ${comopt_DEPENDENCIES} Eigen3::Eigen)

      message(STATUS "${EIGEN_LIBRARY} path : ${DIR}")
    else()
      message(FATAL_ERROR "${EIGEN_LIBRARY} configuration file (Eigen3Config.cmake) not found in ${Eigen3_DIR}")
    endif()
  elseif(DEFINED ENV{EIGEN_DIR})
    set(DIR $ENV{EIGEN_DIR})

    set(EIGEN_INCLUDE_DIR "${DIR}/include/eigen3")

    include_directories(${EIGEN_INCLUDE_DIR})

    message(STATUS "${EIGEN_LIBRARY} path : ${DIR}")
  elseif(NOT "${EIGEN_DIR}" STREQUAL "")
    set(DIR ${EIGEN_DIR})

    set(EIGEN_INCLUDE_DIR "${DIR}/include/eigen3")

    include_directories(${EIGEN_INCLUDE_DIR})

    message(STATUS "${EIGEN_LIBRARY} path : ${DIR}")
  else()
    if(WIN32)
      if(NOT "${CEMEF_DIR}" STREQUAL "")
	set(DIR ${CEMEF_DIR}/eigen/3.3.90)

	include_directories("${DIR}/include/eigen3")

	message(STATUS "${EIGEN_LIBRARY} path : ${DIR}")
      endif()
    endif()
  endif()

  if("${DIR}" STREQUAL "")
    set(EIGEN_DIR "" CACHE PATH "EIGEN path")

    set(${EIGEN_LIBRARY}_GIT_URL "https://github.com/eigenteam/eigen-git-mirror.git" CACHE STRING "Eigen git url")
    set(${EIGEN_LIBRARY}_GIT_TAG "master" CACHE STRING "Eigen git tag")
    set(${EIGEN_LIBRARY}_CMAKE_ARGS "-DBUILD_TESTING=OFF" CACHE STRING "Eigen cmake arguments")

    update_3rdparty(${EIGEN_LIBRARY} ${3rdParty_DIR})

    find_3rdparty(${EIGEN_LIBRARY} Eigen3 "${3rdParty_DIR}/${EIGEN_LIBRARY}-build")

    set(EIGEN_TARGET ${EIGEN_LIBRARY}_3RDPARTY)

    set(EIGEN_INCLUDE_DIR "${3rdParty_DIR}/${EIGEN_LIBRARY}-install/include/eigen3")

    if(TARGET ${EIGEN_LIBRARY}_3RDPARTY)
      set(comopt_DEPENDENCIES_3RDPARTY ${comopt_DEPENDENCIES_3RDPARTY} ${EIGEN_LIBRARY}_3RDPARTY)
    endif()

    if(TARGET Eigen3::Eigen)
      set(comopt_DEPENDENCIES ${comopt_DEPENDENCIES} Eigen3::Eigen)
    endif()

    include_directories(${EIGEN_INCLUDE_DIR})

    message(STATUS "${EIGEN_LIBRARY} path : ${3rdParty_DIR}/${EIGEN_LIBRARY}-install")
  endif()

  ####################################################################
  ## CCache optimizes compilation
  ####################################################################

  find_program(CCACHE_FOUND ccache)
  if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
  endif(CCACHE_FOUND)


####################################################################
