cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
project(GeoAlgo)

INCLUDE(FindPkgConfig)

#set(GEOLIB_INCLUDE GeoLib/include)
set(GEOLIBREF_INCLUDE GeoLibRefactor/include GeoLibRefactor)

#include_directories(${GEOLIB_INCLUDE})
include_directories(${GEOLIBREF_INCLUDE})

#add_subdirectory(GeoLib)
add_subdirectory(GeoLibRefactor)

file( GLOB SOURCE src/*.cpp )

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

add_executable(GeoAlgo ${SOURCE})
target_link_libraries(GeoAlgo GeoLibRefactor ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${GLEW_LIBRARY})