#!/bin/bash
# Number of processors used for compilation
NBPROCS=1

if [ "$1" == "clean" ]; then
  # Clean the build folder
  rm -rf build
elif [ "$1" == "build" ]; then
  # Create the build folder if it doesn't exist
  mkdir -p build
  # Enter the build folder and run cmake and make
  cd build
  cmake ..
  make -j$NBPROCS
elif [ "$1" == "test" ]; then
  # Run ctest in the build folder
  cd build/tests
  ctest --output-on-failure 
elif [ "$1" == "full" ]; then
  # Create the build folder if it doesn't exist
  mkdir -p build
  # Enter the build folder and run cmake and make
  cd build
  cmake ..
  make -j$NBPROCS
  # Run ctest in the build folder
  cd build/tests
  ctest --output-on-failure 
else
  # Print an error message if an invalid argument is provided
  echo "Invalid argument. Please use 'clean', 'build', or 'test'."
fi
