### comoptCompilerFlags.cmake --- 
## 
######################################################################
## 
### Commentary: 
## 
######################################################################

######################################################################
## Windows
######################################################################

if(WIN32)
  add_definitions(/D_LIB /D_CONSOLE /DCOUT /DSTD_IO /DHAVE_NO_VARIABLE_RETURN_TYPE_SUPPORT /D_CRT_SECURE_NO_DEPRECATE /DNDEBUG /DGOOGLE_GLOG_DLL_DECL=)

#  if (NOT BUILD_SHARED_LIBS)
    foreach(LIBRARY ${${PROJECT_NAME}_LIBRARIES})
      string(TOUPPER ${LIBRARY} MACRO_NAME)
      add_definitions(/D${MACRO_NAME}_STATIC_DEFINE)
    endforeach()
#  endif()

  remove_compiler_flags(CMAKE_CXX_FLAGS "/MDd;/MD;/D_WINDOWS;_DEBUG;/MTd;/MT;/Fr;/FR;/D_ITERATOR_DEBUG_LEVEL=0;/D_ITERATOR_DEBUG_LEVEL=1;/D_ITERATOR_DEBUG_LEVEL=2" "CXX Flags")

  add_compiler_flags(CMAKE_CXX_FLAGS "/MT;/MP;/Gy;/Oy-;/GF;/Gm-;/D_ITERATOR_DEBUG_LEVEL=0" "CXX Flags" "GENERAL" "RELEASE")

  add_compiler_flags(CMAKE_CXX_FLAGS "/MTd;/MP;/Gy;/Oy-;/GF;/Gm-;/D_ITERATOR_DEBUG_LEVEL=2" "CXX Flags" "DEBUG")

  add_compiler_flags(CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:CONSOLE" "Executable linker flags")
endif()

######################################################################
## Linux
######################################################################

if(UNIX)
  add_definitions(-DSTD_IO -DDECO -DSUN -DLINUX -DBLASLAPACK_WITH_UNDERSCORE -DHAVE_NO_VARIABLE_RETURN_TYPE_SUPPORT)

  add_compiler_flags(CMAKE_CXX_FLAGS "-std=c++11 -Wall -ldl" "CXX Flags")

  if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    remove_compiler_flags(CMAKE_CXX_FLAGS "-O2;-DNDEBUG" "CXX Flags" GENERAL)
    add_compiler_flags(CMAKE_CXX_FLAGS "-O2 -DNDEBUG" "CXX Flags" GENERAL)
  elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    remove_compiler_flags(CMAKE_CXX_FLAGS "-O2" "CXX Flags" GENERAL)
    remove_compiler_flags(CMAKE_CXX_FLAGS "-O2;-O3;-DNDEBUG" "CXX Flags" RELEASE)
    add_compiler_flags(CMAKE_CXX_FLAGS "-O2 -DNDEBUG" "CXX Flags" RELEASE)
  elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    remove_compiler_flags(CMAKE_CXX_FLAGS "-O2;-DNDEBUG" "CXX Flags" GENERAL)
    remove_compiler_flags(CMAKE_CXX_FLAGS "-g" "CXX Flags" DEBUG)
    add_compiler_flags(CMAKE_CXX_FLAGS "-g" "CXX Flags" DEBUG)
  else()
    remove_compiler_flags(CMAKE_CXX_FLAGS "-O2;-DNDEBUG" "CXX Flags" GENERAL)
  endif()

  if(NOT APPLE)
    add_compiler_flags(CMAKE_CXX_FLAGS "-lgfortran" "CXX Flags")
  endif()

endif()

######################################################################
## Apple
######################################################################

if(APPLE)

endif()
