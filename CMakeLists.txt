cmake_minimum_required(VERSION 3.6)
project(GeoLib)

set(CMAKE_CXX_STANDARD 11)
set(LIB_FILES src/Vector.cpp src/Vector.h src/Mesh.cpp src/Mesh.h src/Triangle.cpp src/Triangle.h)
#set(SOURCE_FILES src/Vector.cpp src/Vector.h test/test.cpp src/Mesh.cpp src/Mesh.h)
#add_executable(Test ${SOURCE_FILES})
find_package(OpenGL REQUIRED)

add_library(GeoLib ${LIB_FILES})
