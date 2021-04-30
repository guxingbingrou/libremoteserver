project(libremoteserver CXX)
cmake_minimum_required(VERSION 2.8.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")
find_package(X11 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(Xext REQUIRED xext)





include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/Base)
add_subdirectory(src)
add_subdirectory(tests)
