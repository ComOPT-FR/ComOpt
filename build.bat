@echo off

:: Compile COMOPT

setlocal EnableDelayedExpansion

:: Git submodule update 
git submodule update --init --recursive

set CMDLINE=%*
:: Default values
set RUN_CMAKE=yes
set RUN_BUILD=yes
set GENERATOR=VisualStudio2019
set BUILD_TYPE=Release
set LIBRARIES_PATHS_FILENAME=
set BUILD_DIR_NAME=build
set CMAKE_ARGUMENTS=

goto main

:SplitArgs
  REM recursive procedure to split off the first two tokens from the input
  if "%*" neq "" (
    REM %%i = KEY, %%j = VALUE, %%k = remainder of input
    REM delimiters are space character and equals character
    for /F "tokens=1,2,* delims== " %%i in ("%*") do call :AssignKeyValue %%i %%j & call :SplitArgs %%k
  )
  goto :eof

:AssignKeyValue
  REM KEY %1, VALUE %2
  if /i %1 equ -t (
    set BUILD_TYPE=%2
  ) else if /i %1 equ -b (
    set RUN_BUILD=%2
  ) else if /i %1 equ -c (
    set RUN_CMAKE=%2
  ) else if /i %1 equ -d (
    set BUILD_DIR_NAME=%2
  ) else if /i %1 equ -g (
    set GENERATOR=%2
  ) else if /i %1 equ -a (
    set CMAKE_ARGUMENTS="%2"
  ) else if /i %1 equ -e (
    set LIBRARIES_PATHS_FILENAME=%2
  ) else (
    REM Append unrecognised [key,value] to BADARGS
    REM echo Unknown KEY %1
    call :usage
    exit /b 1
  )
  goto :eof

:: How to use this batch
:usage
  echo Compile COMOPT
  echo Usage :
  echo ./build.bat
  echo -h     -- display this help
  echo -b     -- build (yes by default)
  echo -t     -- build type (Release by default)
  echo -g     -- generator used (VisualStudio2015 by default)
  echo -c     -- run cmake (yes by default)
  echo -d     -- build directory name (buildWindows by default)
  echo -a     -- cmake arguments (empty by default)
  echo -e     -- filename containing external libraries paths (optional, example in data/scripts/external_libraries_path.txt)
  set EXIT=true
  exit /b 1

:main
set EXIT=false
call :SplitArgs %CMDLINE%

if %EXIT% equ true (
    exit /b 1
)

REM Check generator
if "%GENERATOR%" == "Ninja" (
    set PATH="%PATH%;%PWD%\tools\generator"
    set PATH=!PATH:"=!
    set BUILD_COMMAND=ninja
    set CMAKE_ARGS=-DCMAKE_BUILD_TYPE=%BUILD_TYPE%
    set VISUAL_STUDIO_VERSION=17
) else (
    if "%GENERATOR%" == "VisualStudio2015" (
        set GENERATOR=Visual Studio 14 2015
        set VISUAL_STUDIO_VERSION=14
    ) else if "%GENERATOR%" == "VisualStudio2017" (
        set GENERATOR=Visual Studio 15 2017
        set VISUAL_STUDIO_VERSION=17
	)else if "%GENERATOR%" == "VisualStudio2019" (
        set GENERATOR=Visual Studio 16 2019
        set VISUAL_STUDIO_VERSION=19
	)else if "%GENERATOR%" == "VisualStudio2022" (
        set GENERATOR=Visual Studio 17 2022
        set VISUAL_STUDIO_VERSION=22
    )else (
        call :usage
        exit /b 1
    )
    set BUILD_COMMAND=cmake --build . --target ALL_BUILD --config %BUILD_TYPE%
    set CMAKE_ARGS=
)

REM Check if LIBRARIES_PATHS_FILENAME has been defined and read external libraries paths
if "%LIBRARIES_PATHS_FILENAME%" neq "" (
    for /F "tokens=1,2,* delims== " %%i in (%LIBRARIES_PATHS_FILENAME%) do (
        if /i "%%i:~0,1%" neq "#" (
            set %%i=%%j
        )
    )
)

REM Setting environment for Visual Studio WIN64
set CURRENT_DIR="%cd%"
if "%VISUAL_STUDIO_VERSION%" == "14" (
    call "C:\Program Files (x86)\Microsoft Visual Studio %VISUAL_STUDIO_VERSION%.0\VC\vcvarsall.bat" amd64
) else (
    call "C:\Program Files (x86)\Microsoft Visual Studio\20%VISUAL_STUDIO_VERSION%\Community\VC\Auxiliary\Build\vcvars64.bat"
)
cd %CURRENT_DIR%

REM Creation of build directory
if not exist %BUILD_DIR_NAME% mkdir %BUILD_DIR_NAME%

REM Move in build directory
cd %BUILD_DIR_NAME%

REM Creation of visual studio project
if %RUN_CMAKE% equ yes (
    cmake .. -DCMAKE_CONFIGURATION_TYPES="Release;Debug" -G "%GENERATOR%" %CMAKE_ARGS% %CMAKE_ARGUMENTS%
)

REM Compilation
if %RUN_BUILD% equ yes (
    %BUILD_COMMAND%
)

REM back to initial directory
cd ..

REM Wait before closing output window
pause
