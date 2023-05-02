SDP Formulation for Composite Laminates.

## Dependencies ##
1) to include new dependencies, add them to the vcpkg.json file and run the following command:
2) add include directories to the comoptDependencies.cmake file

```
# add Eigen dependency from vcpkg
find_package(Eigen CONFIG REQUIRED)
message(STATUS "Eigen found: ${Eigen_INCLUDE_DIRS}")
include_directories(${Eigen_INCLUDE_DIRS})
```




