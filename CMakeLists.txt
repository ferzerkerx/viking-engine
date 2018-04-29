cmake_minimum_required(VERSION 3.10)
project(viking_engine)

set(CMAKE_CXX_STANDARD 11)

#file(GLOB_RECURSE sources **/*.cpp **/*.h)
#add_executable(viking_engine ${sources})
add_executable(viking_engine Main.cpp Camara.h Camara.cpp Textura.h Textura.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(viking_engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )