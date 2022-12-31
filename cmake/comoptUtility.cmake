### MacroUtility.cmake --- 
##
######################################################################
## 
### Commentary: 
## 
######################################################################

macro(markVariablesAsAdvanced)
  # Sundials
  mark_as_advanced(${SUNDIALS_LIBRARY}_GIT_URL)
  mark_as_advanced(${SUNDIALS_LIBRARY}_GIT_TAG)
  mark_as_advanced(${SUNDIALS_LIBRARY}_CMAKE_ARGS)
  # glog
  mark_as_advanced(glog_DEPENDS)
  mark_as_advanced(glog_GIT_URL)
  mark_as_advanced(glog_GIT_TAG)
  mark_as_advanced(glog_CMAKE_ARGS)
  # gflags
  mark_as_advanced(gflags_DEPENDS)
  mark_as_advanced(gflags_GIT_URL)
  mark_as_advanced(gflags_GIT_TAG)
  mark_as_advanced(gflags_CMAKE_ARGS)
  # Ceres
  mark_as_advanced(${CERES_LIBRARY}_GIT_URL)
  mark_as_advanced(${CERES_LIBRARY}_GIT_TAG)
  mark_as_advanced(${CERES_LIBRARY}_CMAKE_ARGS)
  mark_as_advanced(${CERES_LIBRARY}_DEPENDS)
  # Swig
  mark_as_advanced(SWIG_DIR)
  mark_as_advanced(SWIG_EXECUTABLE)
  mark_as_advanced(SWIG_VERSION)
  # Eigen
  mark_as_advanced(${EIGEN_LIBRARY}_GIT_URL)
  mark_as_advanced(${EIGEN_LIBRARY}_GIT_TAG)
  mark_as_advanced(${EIGEN_LIBRARY}_CMAKE_ARGS)
  # CMake
  mark_as_advanced(CMAKE_INSTALL_PREFIX)
  mark_as_advanced(CMAKE_BUILD_TYPE)
  mark_as_advanced(CMAKE_CONFIGURATION_TYPES)
  mark_as_advanced(file_cmd)
  mark_as_advanced(CIMLIB_PRECOMPILED_HEADERS)
  mark_as_advanced(MAKE_EXECUTABLE)
  if(APPLE)
    mark_as_advanced(CMAKE_OSX_ARCHITECTURES)
    mark_as_advanced(CMAKE_OSX_DEPLOYMENT_TARGET)
    mark_as_advanced(CMAKE_OSX_SYSROOT)
  endif()
  # CPack
  mark_as_advanced(CPACK_SOURCE_PACKAGE_FILE_NAME)
  mark_as_advanced(CPACK_PACKAGING_INSTALL_PREFIX)
  # CCache
  mark_as_advanced(CCACHE_FOUND)
endmacro()

######################################################################

macro(add_compiler_flags var flags title)
  ## Add a flag to given variable

  string(LENGTH "${flags}" FLAGS_SIZE)

  if(${FLAGS_SIZE} GREATER 0)
    set(versions ${ARGN})

    if("${versions}" STREQUAL "")
      set(versions "GENERAL" "RELEASE" "DEBUG")
    endif()

    foreach(version ${versions})
      if("${version}" STREQUAL "GENERAL")
	set(version)
      else()
	set(version _${version})
      endif()

      set(complete_var "${var}${version}")

      foreach(flag ${flags})
	if("${${complete_var}}" STREQUAL "")
	  set(${complete_var} "${flag}" CACHE STRING "${title}" FORCE)
	else()
	  string(FIND ${${complete_var}} ${flag} FOUND)
	  if (${FOUND} EQUAL -1)
	    set(${complete_var} "${${complete_var}} ${flag}" CACHE STRING "${title}" FORCE)
	  endif()
	endif()
      endforeach()
    endforeach()
  endif()

endmacro()

######################################################################

macro(remove_compiler_flags var flags title)
  ## Remove a flag from given variable

  string(LENGTH "${flags}" FLAGS_SIZE)

  if(${FLAGS_SIZE} GREATER 0)
    set(versions ${ARGN})

    if("${versions}" STREQUAL "")
      set(versions "GENERAL" "RELEASE" "DEBUG" "MINSIZEREL" "RELWITHDEBINFO")
    endif()

    foreach(version ${versions})
      if("${version}" STREQUAL "GENERAL")
	set(version)
      else()
	set(version _${version})
      endif()

      set(complete_var "${var}${version}")

      foreach(flag ${flags})
	if(NOT "${${complete_var}}" STREQUAL "")
          string(REPLACE "${flag}" "" ${complete_var} ${${complete_var}})
          set(${complete_var} "${${complete_var}}" CACHE STRING "${title}" FORCE)
	endif()
      endforeach()
    endforeach()
  endif()

endmacro()

######################################################################

macro(list_subdirectories_with_sources curdir names paths)
  ## Scans the current directory and returns a list of subdirectories
  ## that contains headers (*.h). Keep only directory matching a name of "names" list

  unset(new_list)
  file(GLOB_RECURSE list_h RELATIVE ${curdir} ${curdir}/*.h)
  list(APPEND new_list ${list_h})
  file(GLOB_RECURSE list_hpp RELATIVE ${curdir} ${curdir}/*.hpp)
  list(APPEND new_list ${list_hpp})
  file(GLOB_RECURSE list_cc RELATIVE ${curdir} ${curdir}/*.cc)
  list(APPEND new_list ${list_cc})
  file(GLOB_RECURSE list_cpp RELATIVE ${curdir} ${curdir}/*.cpp)
  list(APPEND new_list ${list_cpp})
  file(GLOB_RECURSE list_c RELATIVE ${curdir} ${curdir}/*.c)
  list(APPEND new_list ${list_c})
  file(GLOB_RECURSE list_f RELATIVE ${curdir} ${curdir}/*.f)
  list(APPEND new_list ${list_f})
  file(GLOB_RECURSE list_f90 RELATIVE ${curdir} ${curdir}/*.f90)
  list(APPEND new_list ${list_f90})
  set(paths "")
  set(paths_list "")
  foreach(file_path ${new_list})
    get_filename_component(path ${file_path} PATH)
    if(NOT "${path}" STREQUAL "")
      if (NOT ${path} IN_LIST ${paths})
	get_filename_component(name ${path} NAME)
	if (";${names};" MATCHES ";${name};")
	  set(paths_list ${paths_list} ${curdir}/${path})
	endif()
	set(paths ${paths} ${path})
      endif()
    endif()
  endforeach()

  list(REMOVE_DUPLICATES paths_list)

  set(${paths} ${paths_list})
endmacro()

######################################################################

macro(list_subdirectories_containing curdir return_list file)
  ## Scans the current directory and returns a list of subdirectories
  ## (from the current directory) that contains the given file

  file(GLOB_RECURSE new_list RELATIVE ${curdir} ${curdir}/${file})
  set(dir_list "")
  foreach(file_path ${new_list})
    get_filename_component(dir_path ${file_path} PATH)
    if(NOT "${dir_path}" STREQUAL "")
      set(dir_list ${dir_list} ${dir_path})
    endif()
  endforeach()
  list(REMOVE_DUPLICATES dir_list)
  set(${return_list} ${dir_list})
endmacro()

######################################################################

macro(copy_and_install_lib dir patterns)

  if(WIN32)
    set(DIR_INSTALL bin)
  else()
    set(DIR_INSTALL lib)
  endif()

  set(FILES_SYMLINK)
  foreach(DIR ${dir})
    foreach(pattern ${patterns})
      file(GLOB_RECURSE FILES ${DIR}/${pattern})
      foreach(FILE ${FILES})
        if(NOT IS_SYMLINK ${FILE})
          if(APPLE)
            file(COPY ${FILE} DESTINATION ${CMAKE_BINARY_DIR}/${DIR_INSTALL})
          endif()
          install(FILES ${FILE} DESTINATION ${DIR_INSTALL} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
        else()
          list(APPEND FILES_SYMLINK "${FILE}")
        endif()
      endforeach()
    endforeach()
  endforeach()

  foreach(FILE ${FILES_SYMLINK})
    if(APPLE)
      file(COPY ${FILE} DESTINATION ${CMAKE_BINARY_DIR}/${DIR_INSTALL})
    endif()
    install(FILES ${FILE} DESTINATION ${DIR_INSTALL} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
  endforeach()

endmacro()

######################################################################

macro(is_empty path resultContent)
    if(IS_DIRECTORY "${path}")
        file(GLOB ${resultContent} "${path}/*")
        #message("${path} is a directory and contain : ${resultContent}")
    elseif(EXISTS "${path}")
        file(READ ${resultContent} "${path}")
        #message("${path} is a file and contain : ${resultContent}")
    endif()
endmacro()

######################################################################

macro(install_include dir patterns)

  foreach(DIR ${dir})
    foreach(pattern ${patterns})
      file(GLOB FILES ${DIR}/${pattern})
      foreach(FILE ${FILES})
        install(FILES ${FILE} DESTINATION include)
      endforeach()
    endforeach()
  endforeach()

endmacro()

######################################################################

macro(check_ssh_key)

  if(WIN32)
    find_package(Git REQUIRED)

    # SSH private key is needed to download git project
    if("${SSH_PRIVATE_KEY}" STREQUAL "" OR NOT EXISTS "${SSH_PRIVATE_KEY}")
      set(SSH_FILENAME "c:/local/$ENV{USERNAME}/.ssh/id_rsa")
      if(NOT EXISTS "${SSH_FILENAME}")
	set(SSH_FILENAME "c:/users/$ENV{USERNAME}/.ssh/id_rsa")
      endif()
    endif()

    set(SSH_PRIVATE_KEY ${SSH_FILENAME} CACHE FILEPATH "SSH private key (needed by git to download an external project)")

    if(NOT EXISTS ${SSH_PRIVATE_KEY})
      message(FATAL_ERROR "SSH private key must be defined")
    endif()
  endif()

endmacro()

######################################################################