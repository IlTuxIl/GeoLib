cmake_minimum_required(VERSION 3.6)
project(GeoLib)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/Vector.cpp src/Vector.h test/testVector.cpp)
add_executable(untitled ${SOURCE_FILES})
#add_library(GeoLib ${SOURCE_FILES})