cmake_minimum_required(VERSION 3.6)
project(GeoLib)

set(CMAKE_CXX_STANDARD 11)
set(LIB_FILES src/Vector.cpp src/Vector.h src/Mesh.cpp src/Mesh.h src/Triangle.cpp src/Triangle.h src/TriangleTopo.cpp src/TriangleTopo.h src/Sommet.cpp src/Sommet.h src/Triangulation.cpp src/Triangulation.h)
set(SOURCE_FILES test/test.cpp src/Vector.cpp src/Vector.h src/Mesh.cpp src/Mesh.h src/Triangle.cpp src/Triangle.h src/TriangleTopo.cpp src/TriangleTopo.h src/Sommet.cpp src/Sommet.h src/Triangulation.cpp src/Triangulation.h)
include_directories(Test ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
add_executable(Test ${SOURCE_FILES})
target_link_libraries(Test ${OPENGL_LIBRARIES})
find_package(OpenGL REQUIRED)

add_library(GeoLib ${LIB_FILES})
