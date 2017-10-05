cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
project(GeoAlgo)

INCLUDE(FindPkgConfig)

set(GEOLIB_INCLUDE GeoLib/include)
#set(GKIT_INCLUDE GeoLib/gKit/include)

add_subdirectory(GeoLib)

file( GLOB SOURCE src/*.cpp )

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(${GEOLIB_INCLUDE})
#include_directories(${GKIT_INCLUDE})

add_executable(GeoAlgo ${SOURCE})
target_link_libraries(GeoAlgo GeoLib ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${GLEW_LIBRARY})