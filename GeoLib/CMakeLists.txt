cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
project(GeoLib)

file(GLOB GKIT_SOURCES gKit/*.cpp)

include_directories(include)
file( GLOB SOURCE src/*.cpp include/*.h)

find_package(GLEW REQUIRED)
add_library(GeoLib ${SOURCE} ${GKIT_SOURCES})