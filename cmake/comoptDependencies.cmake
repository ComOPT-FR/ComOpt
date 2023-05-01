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

####################################################################
## Non linear solvers
####################################################################

# if(USE_SUNDIALS)
#   unset(DIR)

#   if(DEFINED ENV{SUNDIALS_DIR})
#     set(DIR $ENV{SUNDIALS_DIR})
#   elseif(NOT "${SUNDIALS_DIR}" STREQUAL "")
#     set(DIR ${SUNDIALS_DIR})
#   else()
#     if(WIN32)
#       if(NOT "${CEMEF_DIR}" STREQUAL "")
# 	set(DIR ${CEMEF_DIR}/sundials/Visual2015)
#       endif()
#     endif()
#   endif()

#   if("${SUNDIALS_DIR}" STREQUAL "")
#     set(SUNDIALS_DIR ${DIR} CACHE PATH "Sundials path" FORCE)
#   else()
#     set(SUNDIALS_DIR ${DIR} CACHE PATH "Sundials path")
#   endif()

#   set(SUNDIALS_LIBRARY sundials)

#   if("${SUNDIALS_DIR}" STREQUAL "")
#     set(SUNDIALS_DIR "" CACHE PATH "Sundials path")

#     check_ssh_key()

#     set(${SUNDIALS_LIBRARY}_GIT_URL "git@git.sophia.mines-paristech.fr:CIMLIB/sundials.git" CACHE STRING "Sundials git url")
#     set(${SUNDIALS_LIBRARY}_GIT_TAG "update_v3" CACHE STRING "Sundials git tag")
#     set(${SUNDIALS_LIBRARY}_CMAKE_ARGS "-DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=ON -DBUILD_CVODE=OFF -DBUILD_CVODES=OFF -DBUILD_IDA=OFF -DBUILD_IDAS=OFF -DBUILD_KINSOL=ON -DBUILD_ARKODE=OFF -DEXAMPLES_ENABLE_C=FALSE -DCMAKE_BUILD_TYPE=Release -DCMAKE_USER_MAKE_RULES_OVERRIDE=${CMAKE_SOURCE_DIR}/cmake/comoptCompilerFlagsOverrides.cmake" CACHE STRING "Sundials cmake arguments")

#     update_3rdparty(${SUNDIALS_LIBRARY} ${3rdParty_DIR})

#     find_3rdparty(${SUNDIALS_LIBRARY} ${SUNDIALS_LIBRARY} "${3rdParty_DIR}/${SUNDIALS_LIBRARY}-build")

#     if(TARGET ${SUNDIALS_LIBRARY}_3RDPARTY)
#       set(comopt_DEPENDENCIES_3RDPARTY ${comopt_DEPENDENCIES_3RDPARTY} ${SUNDIALS_LIBRARY}_3RDPARTY)
#     endif()

#     if(${SUNDIALS_LIBRARY}_USE_FILE)
#       include(${${SUNDIALS_LIBRARY}_USE_FILE})
#     endif()

#     set(SUNDIALS_DIR "${3rdParty_DIR}/${SUNDIALS_LIBRARY}-install" CACHE PATH "Sundials path" FORCE)
#   endif()

#   message(STATUS "${SUNDIALS_LIBRARY} path : ${SUNDIALS_DIR}")

#   set(SUNDIALS_INSTALL_DIR ${SUNDIALS_DIR})
#   set(SUNDIALS_INCLUDE_DIR ${SUNDIALS_DIR}/include)
#   set(SUNDIALS_LIBRARY_DIR ${SUNDIALS_DIR}/lib)
#   set(SUNDIALS_BINARY_DIR ${SUNDIALS_DIR}/bin)

#   include_directories(${SUNDIALS_INCLUDE_DIR})
#   link_directories(${SUNDIALS_LIBRARY_DIR})

#   set(comopt_DEPENDENCIES ${comopt_DEPENDENCIES} sundials_kinsol sundials_nvecserial)
# endif()

####################################################################
## Numerical Derivatives
####################################################################

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

  # Boost dependency


####################################################################
