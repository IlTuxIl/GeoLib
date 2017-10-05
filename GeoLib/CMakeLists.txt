cmake_minimum_required(VERSION 3.8)
project(GeoLib)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

#INCLUDE(FindPkgConfig)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
#include_directories(gKit ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

add_subdirectory(gKit)

set(LIB_FILES src/Vector.cpp include/Vector.h src/Mesh.cpp include/Mesh.h src/Triangle.cpp include/Triangle.h src/TriangleTopo.cpp include/TriangleTopo.h src/Sommet.cpp include/Sommet.h src/Triangulation.cpp include/Triangulation.h)
add_library(GeoLib ${LIB_FILES})
