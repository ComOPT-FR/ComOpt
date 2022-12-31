include(CMakeParseArguments)

## Need cmaketools !
include(comoptUtility) # to get the is_empty function (from cmaketools)

## CMAKE_DOCUMENTATION_START EXTERNAL_3RDPARTY
##
### EXTERNAL_3RDPARTY macro --- Auto get & build & install a 3rdParty project\\n
## ONLY FOR GIT PROJECT AT THIS TIME... \\n
##\\n
## Use ExternalProjects CMake Module to auto :\\n
##      - download the 3rdParty project \\n
##      - configure and generate the rdParty project \\n
##      - build and install the 3rdParty project \\n
##\\n
## The default steps for ExternalProject_Add are the following : \\n
## 1) mkdir :       contain the externalProject                                     \\n
## 2) download :    dl the project (using GIT / CVS SVN or compress file from URL)  \\n
## 3) update :      update the project by recover the last version from URL         \\n
## 4) patch :       allow to add custom command                                     \\n
## 5) configure :   configure the external project like using cmake                 \\n
## 6) build :       build the external project like using make/nmake                \\n
## 7) test :        allow to execute test command before or after the install step  \\n
## 8) install :     install external project                                        \\n
## 9) complete/done:to mark the external project "up to date".                      \\n
##\\n
## We can custom each step to provide more custom command on each step with ExternalProject_Add_Step.\\n
##\\n
## PIPELINE:    In order to have an auto download and "auto find" dependences libs/include
##              we have to run cmake twice. Indeed, as we want to download dependences and
##              configure the SuperProject in the same time with the same CMakeList.txt, at 
##              make time, we have to run the CMakeList.txt twice : \\n
##              The first time to configure makeFiles for the 3rdParty (using externalProject).\\n
##              The second time to configure makeFiles to allow SuperProject find its dependences.\\n
##\\n
## All <3rdParty>EP_* are variables using only internaly for the externalProject macros.\\n
## The <3rdParty>_EP is the custom target which represent the 3rdParty project we used
## the <packageName>_3RDPARTY is the result target convention for your external project.
##
## Usage :
## \\code
## include(3rdParty.cmake)\n
## EXTERNAL_3RDPARTY(OSGVR                                                                                      \n
##                   URL            "ssh+git://scm.gforge.inria.fr/gitroot/osgvr/osgvr.git"                     \n
##                   SOURCE         "${CMAKE_SOURCE_DIR}/3rdParty"      #Where download/configure 3rdParty      \n
##                   DESTINATION    "${CMAKE_BINARY_DIR}/3rdParty"      #Where build/install 3rdParty           \n
##                   CMAKE_ARGS     "-DVERBOSE=true"    #cmake arguments for 3rdParty project when run cmake    \n
##                   LOG            true        #export each output step in files                               \n
##                   CACHED         true        #put args of this macro in cache                                \n
##                   RERUN_CMAKE    true        #re run cmake                                                   \n
## )
## \\endcode
##\\n
## SOURCE         "DirPath"      #Where download/configure 3rdParty      \\n
## DESTINATION    "DirPath"      #Where build/install 3rdParty           \\n
## CMAKE_ARGS     "-DCMakeVariables"    #cmake arguments for 3rdParty project when run cmake    \\n
## LOG            bool        #export each output step in files                               \\n
##\\n
## For your CMAKE_ARGS, it's not necessary to defined the INSTALL_PREFIX as the DESTINATION variable do it for you.\\n
## are mapped from your super-project to the 3rdParty project.
##
## CMAKE_DOCUMENTATION_END
function(EXTERNAL_3RDPARTY packageName)

    ## aditional OPTIONS for this macro
    cmake_parse_arguments(${packageName} "" "URL;GIT_TAG;SOURCE;DESTINATION;RERUN_CMAKE;LOG;CACHED" "CMAKE_ARGS;DEPENDS" ${ARGN} )

    ## default CACHED 
    ## be careful, with all cache entry, if you want to change its value from another way that the GUI,
    ## don't forget to "remove entry" in the GUI to recover the corret value the next time you use the GUI.
    if( NOT ${packageName}_CACHED  )
        set(${packageName}_CACHED false )
    endif()
    if(${packageName}_CACHED)
        set(${packageName}_DESTINATION    ${${packageName}_DESTINATION}    CACHE PATH   "Where build/install 3rdParty")
        set(${packageName}_SOURCE         ${${packageName}_SOURCE}         CACHE PATH   "Where download/configure 3rdParty")
        set(${packageName}_CMAKE_ARGS     ${${packageName}_CMAKE_ARGS}     CACHE STRING "cmake arguments for 3rdParty project")
        set(${packageName}_URL            ${${packageName}_URL}            CACHE STRING "the url to download 3rdParty")
        set(${packageName}_GIT_TAG        ${${packageName}_GIT_TAG}        CACHE STRING "the tag to download 3rdParty")

        set_property(CACHE ${packageName}_CMAKE_ARGS       APPEND PROPERTY TYPE STRING)

        mark_as_advanced(${packageName}_DESTINATION)
        mark_as_advanced(${packageName}_SOURCE)
        mark_as_advanced(${packageName}_URL)
        mark_as_advanced(${packageName}_GIT_TAG)
    endif()

    ## default DESTINATION dir
    if( NOT ${packageName}_DESTINATION )
      set(${packageName}_DESTINATION "${CMAKE_BINARY_DIR}/3rdParty")        
    endif()

    ## default DEPENDS dir
    if( NOT ${packageName}_DEPENDS )
      set(${packageName}_DEPENDS "")        
    endif()

    ## default GIT_TAG dir
    if( NOT ${packageName}_GIT_TAG )
        set(${packageName}_GIT_TAG "")
    endif()

    ## default RERUN_CMAKE
    if( NOT ${packageName}_RERUN_CMAKE )
      set(${packageName}_RERUN_CMAKE OFF)
    endif()

    ## default URL dir
    if( NOT ${packageName}_URL )
      set(${packageName}_URL "")
    endif()

    ## default SOURCE dir
    if( NOT ${packageName}_SOURCE )
        set(${packageName}_SOURCE ${${packageName}_DESTINATION})
    endif()
    
    ## default CMAKE_ARGS for 3rdParty CMakeLists.txt
    if( NOT ${packageName}_CMAKE_ARGS )
        set(${packageName}_CMAKE_ARGS "" )
    else()
        # Use filter : space separator for this field
        string(REGEX REPLACE " " ";" ${packageName}_CMAKE_ARGS ${${packageName}_CMAKE_ARGS})
    endif()

    ## default LOG
    if( NOT ${packageName}_LOG  )
        set(${packageName}_LOG true )
    endif()
    
    include(ExternalProject)
 
    ############################################################################################
    #################### INIT VARIABLES FOR EXTERNAL PROJECT ###################################
    ############################################################################################
    set(${packageName}_GIT_REPO_URL   "${${packageName}_URL}")
    set(${packageName}_GIT_TAG        "${${packageName}_GIT_TAG}")
    set(${packageName}_DOWNLOAD_DIR   "${${packageName}_SOURCE}")
    set(${packageName}_SOURCE_DIR     "${${packageName}_SOURCE}")
    set(${packageName}_STAMP_DIR      "${${packageName}_DESTINATION}/${packageName}-stamp") # Create a file at each validate step
    set(${packageName}_TMP_DIR        "${${packageName}_DESTINATION}/${packageName}-tmp")   # Intermediate scripts used internly
    set(${packageName}_BINARY_DIR     "${${packageName}_DESTINATION}/${packageName}-build")
    set(${packageName}_INSTALL_DIR    "${${packageName}_DESTINATION}/${packageName}-install")
    
    ## final CMAKE_ARGS
    set(${packageName}_CMAKE_ARGS     
      "${${packageName}_CMAKE_ARGS}"            
      "-DCMAKE_INSTALL_PREFIX=${${packageName}_INSTALL_DIR}"      
    )
    
    ## LOG
    if(${packageName}_LOG)
      set(LOG_3RDPARTY
        LOG_DOWNLOAD    1
        LOG_UPDATE      1
        LOG_CONFIGURE   1
        LOG_BUILD       1
        LOG_INSTALL     1
        )
    endif()

    ############################################################################################
    #################### EXTERNAL PROJECT ADD ##################################################
    ############################################################################################
    ExternalProject_Add(    ${packageName}_3RDPARTY
        PREFIX              ${${packageName}_DESTINATION}
        TMP_DIR             ${${packageName}_TMP_DIR} 
        STAMP_DIR           ${${packageName}_STAMP_DIR}
       ##--Depends step---------------
        DEPENDS             ${${packageName}_DEPENDS}
       ##--Download step--------------
        DOWNLOAD_DIR        ${${packageName}_DOWNLOAD_DIR}
        GIT_REPOSITORY      ${${packageName}_GIT_REPO_URL}
        GIT_TAG             ${${packageName}_GIT_TAG}
       ##--Update step-------------
       UPDATE_COMMAND       "git;status"
       ##--Configure step-------------
        SOURCE_DIR          ${${packageName}_SOURCE_DIR}
        CMAKE_ARGS          ${${packageName}_CMAKE_ARGS}
        #CMAKE_CACHE_ARGS
       ##--Build step-----------------
        BINARY_DIR          ${${packageName}_BINARY_DIR}
       ##--Install step---------------
        INSTALL_DIR         ${${packageName}_INSTALL_DIR}
       ##--Test step------------------
       ##--Output logging-------------
        ${LOG_3RDPARTY}
       ##--Custom targets-------------
        ALWAYS 0
    )

    ############################################################################################
    #################### CUSTOM STEPS ##########################################################
    ############################################################################################
    #--Custom configure step (after update step and before standard configure step )--
    if(NOT WIN32) 
      ExternalProject_Add_Step(${packageName}_3RDPARTY after_patch
	COMMENT "--- update finished. Start configure ---"
	WORKING_DIRECTORY ${${packageName}_BINARY_DIR}
	COMMAND CC=gcc CXX=g++ FC=gfortran ${CMAKE_COMMAND} ${${packageName}_SOURCE_DIR} ${${packageName}_CMAKE_ARGS}
	DEPENDEES update
	DEPENDERS configure
	ALWAYS 0
	)
    endif()
    #--Custom build step (after configure step and before standard build step )--
    ExternalProject_Add_Step(${packageName}_3RDPARTY after_configure
        COMMENT "--- configure finished. Start build : Please wait until the next step ---"
        DEPENDEES configure
        DEPENDERS build
        ALWAYS 0
    )

    ## Work around check the end process of the ExternalProject_Add cmd using output logfiles
    ## Get a list of all <3rdPartyTarget>-*-err.log files an see if we found some "errors" pattern
    if( ${packageName}_LOG )
        set(${packageName}_3RDPARTY_CHECK_CODE
            "
            file(GLOB errLogs \"${${packageName}_STAMP_DIR}/${packageName}_3RDPARTY-*-err.log\")
            message(STATUS \"check file path : ${${packageName}_STAMP_DIR}...\")
            foreach( errLog \${errLogs})
                file(READ \${errLog} contents) 
                string(REGEX MATCH \".?error*\" output \"\${contents}\")
                if(output)
                    message(\"${contents}\")
                    message(WARNING \"ExternalProject encountered an error : see \${errLog}\")
                endif()
            endforeach()
            "
        )
        if(NOT EXISTS ${${packageName}_STAMP_DIR}/${packageName}_3RDPARTY_check.cmake)
            file(WRITE ${${packageName}_STAMP_DIR}/${packageName}_3RDPARTY_check.cmake ${${packageName}_3RDPARTY_CHECK_CODE} )
        endif()

        ## To get the right order (first finished 3rdParty targets)
        ExternalProject_Add_Step(${packageName}_3RDPARTY check
            COMMENT "--- Check the 3rdParty project end process ---"
            COMMAND     ${CMAKE_COMMAND} -P ${${packageName}_STAMP_DIR}/${packageName}_3RDPARTY_check.cmake
            DEPENDEES   install
            ALWAYS 0
        )
    endif()


    ## To get the right order (first finished 3rdParty targets before start to rerun cmake for the current project)
    if( ${packageName}_LOG )
        set(DependeesRerunCmakeTarget check)
    else()
        set(DependeesRerunCmakeTarget install)
    endif()

    ## As ExternalProject_Add function is effective at the make time,
    ## we can't find 3rdParty at configure time (cannot use FindPackage)
    ## Work around re-run CMake just after 3rdParty installation is finished...
    ## So the first pass of cmake->make will install 3rdPArty
    ## and launch the second pass of cmake->make witch auto find 3rdParty (using our macro above)
    ## and update the makeFiles for the superProject
    if(${${packageName}_RERUN_CMAKE})
      ExternalProject_Add_Step(${packageName}_3RDPARTY reruncmake
        COMMENT "--- Start re-run CMake ---"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND     ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR}
        DEPENDEES   ${DependeesRerunCmakeTarget}
        ALWAYS 0
        #LOG 1
      )
    endif()

endfunction()


## CMAKE_DOCUMENTATION_START get_3rdparty_source_dir
##
## internal use. \\n
##\\code
## get_3rdparty_source_dir(packageName packageSrcDir_ret)
##\\encode
## Provide a packageSrcDir_ret from the packageName
##
## CMAKE_DOCUMENTATION_END
MACRO(GET_3RDPARTY_SOURCE_DIR packageName packageSrcDir_ret)
    include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
    # <packageName>_3RDPARTY is the target convention
    ExternalProject_Get_Property(${packageName}_3RDPARTY source_dir)
    set(${packageSrcDir_ret} ${source_dir})
ENDMACRO()


## CMAKE_DOCUMENTATION_START get_3rdparty_binary_dir
##
## internal use. \\n
##\\code
## get_3rdparty_binary_dir(packageName packageBinDir_ret)
##\\encode
## Provide a packageBinDir_ret from the packageName
##
## CMAKE_DOCUMENTATION_END
MACRO(GET_3RDPARTY_BINARY_DIR packageName packageBinDir_ret)
    include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
    # <packageName>_3RDPARTY is the target convention
    ExternalProject_Get_Property(${packageName}_3RDPARTY binary_dir)
    set(${packageBinDir_ret} ${binary_dir})
ENDMACRO()


## CMAKE_DOCUMENTATION_START get_3rdparty_install_dir
##
## internal use. \\n
##\\code
## get_3rdparty_install_dir(packageName packageInstDir_ret)
##\\encode
## Provide a packageInstDir_ret from the packageName
##
## CMAKE_DOCUMENTATION_END
MACRO(GET_3RDPARTY_INSTALL_DIR packageName packageInstDir_ret)
    include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
    # <packageName>_3RDPARTY is the target convention
    ExternalProject_Get_Property(${packageName}_3RDPARTY install_dir)
    set(${packageInstDir_ret} ${install_dir})
ENDMACRO()


## CMAKE_DOCUMENTATION_START find_3rdparty
##
##\\code
## find_3rdparty(packageName)
##\\encode
## Use exactly like the find_package().\\n
## It set the <packageName>_DIR to find the 3RDPARTY package.
##
## CMAKE_DOCUMENTATION_END 
MACRO(FIND_3RDPARTY packageName packageNickName dir)
    set(${packageName}_DIR ${dir})

    IS_EMPTY(${${packageName}_DIR} contentDir) 

    if(contentDir)
      set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${${packageName}_DIR})
      if (EXISTS ${${packageName}_DIR}/${packageNickName}Config.cmake)
        include(${packageNickName}Config)
      endif()
      if (EXISTS ${${packageName}_DIR}/${packageNickName}-config.cmake)
        include(${packageNickName}-config)
      endif()
    endif()

ENDMACRO()

## CMAKE_DOCUMENTATION_START installation_3rdparty
##
## Allow to install the 3rdParty packageName to the given installPrefix.
##\\code
##installation_3rdparty(packageName installPrefix [CUSTOM_TARGET])
##\\encode
## INST_CUSTOM_TARGET :\\n
## ->to true: create a separate target for the 3rdParty installation : then use make install-<packageName>.\\n 
## ->to false: install 3rdParty at make install time (with the superproject installation).\\n
##
## CMAKE_DOCUMENTATION_END 
function(INSTALLATION_3RDPARTY packageName installPrefix)

    cmake_parse_arguments(${packageName} "" "INST_CUSTOM_TARGET" "" ${ARGN} )

    # default CUSTOM_TARGET
    if(NOT DEFINED ${packageName}_INST_CUSTOM_TARGET)
        set(${packageName}_INST_CUSTOM_TARGET true)
    endif()

    include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

    GET_3RDPARTY_SOURCE_DIR(${packageName} ${packageName}_SOURCE_DIR)
    GET_3RDPARTY_BINARY_DIR(${packageName} ${packageName}_BINARY_DIR)
    GET_3RDPARTY_INSTALL_DIR(${packageName} ${packageName}_INSTALL_DIR)

    #message(STATUS "Install ${packageName} 3rdParty project in : ${installPrefix}")
    set(${packageName}_INSTALL_CODE 
        "
        ## Re run 3rdParty CMake in order to re-install 3rdParty
        message(\"----Install ${packageName} to ${installPrefix} !\")
        execute_process(COMMAND ${CMAKE_COMMAND} 
                    -DCMAKE_INSTALL_PREFIX=${installPrefix}
                    ${${packageName}_SOURCE_DIR}
                    --build ${${packageName}_BINARY_DIR}
        )
        execute_process(WORKING_DIRECTORY ${${packageName}_BINARY_DIR}
                        COMMAND ${CMAKE_COMMAND} -P cmake_install.cmake
        )
        
        ## Provide a 3rdParty install manifest file used for uninstall
        file(READ  \"${${packageName}_BINARY_DIR}/install_manifest.txt\" install_manifest_content)
        file(WRITE \"${CMAKE_BINARY_DIR}/${packageName}_install_manifest.txt\" \"\${install_manifest_content}\")
        
        ## Re run 3rdParty CMake in order to reinit the 3rdParty installation config
        execute_process(COMMAND ${CMAKE_COMMAND} 
                    -DCMAKE_INSTALL_PREFIX=${${packageName}_INSTALL_DIR}
                    ${${packageName}_SOURCE_DIR}
                    --build ${${packageName}_BINARY_DIR}
        )
        "
    )

    if(NOT ${packageName}_INST_CUSTOM_TARGET)
        ## used to embeded  3rdParty installation into the install target (make install)
        install(CODE ${${packageName}_INSTALL_CODE} )
        #message(STATUS "***INFO***: Your install target will also install ${packageName} 3RDPARTY to : ${installPrefix}")
    else()
        ## used to create a 3rdParty target installation (make install-<packageName>)
        file(WRITE ${CMAKE_BINARY_DIR}/install-${packageName}.cmake ${${packageName}_INSTALL_CODE} )
        add_custom_target(install-${packageName}
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/install-${packageName}.cmake )
        #message(STATUS "***INFO***: You can use make install-${packageName} to install ${packageName} 3RDPARTY to  : ${installPrefix}")
    endif()

endfunction()

macro(UPDATE_3RDPARTY NAME 3rdParty_DIR)

  if(WIN32)
    set(${NAME}_clone "${3rdParty_DIR}/${NAME}")
    file(GLOB GIT_FILES "${${NAME}_clone}/*")
    if("${GIT_FILES}" STREQUAL "")
      set(GIT_URL ${${NAME}_GIT_URL})
      set(GIT_TAG ${${NAME}_GIT_TAG})
      set(GIT_PROJECT_NAME "${NAME}")

      configure_file("${CMAKE_SOURCE_DIR}/cmake/gitCLone.bat.in" "${3rdParty_DIR}/gitClone.bat" IMMEDIATE @ONLY)

      execute_process(
	COMMAND "gitClone.bat"
	WORKING_DIRECTORY "${3rdParty_DIR}"
	ERROR_VARIABLE ERR
	)
    endif()
  else()
    set(GIT_COMMAND
      URL ${${NAME}_GIT_URL}
      GIT_TAG ${${NAME}_GIT_TAG}
      )
  endif()

  set(${NAME}_download "${3rdParty_DIR}/${NAME}-stamp/${NAME}_3RDPARTY-download")
  set(${NAME}_configure "${3rdParty_DIR}/${NAME}-stamp/${NAME}_3RDPARTY-configure")
  if (NOT EXISTS ${${NAME}_download} OR NOT EXISTS ${${NAME}_configure})

    external_3rdparty(${NAME} ${NAME}
      ${GIT_COMMAND}
      SOURCE "${3rdParty_DIR}/${NAME}"
      CMAKE_ARGS ${${NAME}_CMAKE_ARGS}
      RERUN_CMAKE 1
      )

    installation_3rdparty(${NAME} ${CMAKE_INSTALL_PREFIX} INST_CUSTOM_TARGET false)

    get_3rdparty_install_dir(${NAME} ${NAME}_INSTALL_DIR)

    include_directories(${${NAME}_INSTALL_DIR}/include)
    link_directories(${${NAME}_INSTALL_DIR}/lib)

  else()

    set(EXT ${CMAKE_SHARED_LIBRARY_SUFFIX})

    set(${NAME}_INSTALL_DIR "${3rdParty_DIR}/${NAME}-install")

    if(WIN32)
      file(GLOB_RECURSE FILE ${${NAME}_INSTALL_DIR}/bin/*.dll)
      file(COPY ${FILE} DESTINATION ${CMAKE_BINARY_DIR}/bin)
      install(FILES ${FILE} DESTINATION bin)
    else()
      copy_and_install_lib("${${NAME}_INSTALL_DIR}/lib" "lib*${EXT}*")
    endif()

  endif()

  add_custom_target(update-${NAME}
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${3rdParty_DIR}/${NAME}"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${3rdParty_DIR}/${NAME}-stamp"
    COMMAND ${CMAKE_COMMAND} -E remove ${${NAME}_download}
    COMMAND ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Update ${NAME} 3rdParty"
    )

  add_custom_target(configure-${NAME}
    COMMAND ${CMAKE_COMMAND} -E remove ${${NAME}_configure}
    COMMAND ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Configure ${NAME} 3rdParty"
    )

endmacro()
