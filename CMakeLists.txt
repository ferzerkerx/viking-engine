cmake_minimum_required(VERSION 3.10)
project(viking_engine)

set(CMAKE_CXX_STANDARD 11)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )


add_executable(viking_engine viking/Main.cpp)
target_include_directories(viking_engine PRIVATE framework)
