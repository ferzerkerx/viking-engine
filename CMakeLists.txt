cmake_minimum_required(VERSION 3.10)
project(viking_engine)

set(CMAKE_CXX_STANDARD 11)

#file(GLOB_RECURSE sources **/*.cpp **/*.h)
#add_executable(viking_engine ${sources})
add_executable(viking_engine src/Main.cpp
        src/Camara.h
        src/Camara.cpp
        src/sky/SkyBox.h
        src/sky/SkyBox.cpp
        src/Fuente.h
        src/Fuente.cpp
        src/Timer.h
        src/Timer.cpp
        src/Textura.h
        src/Textura.cpp
        src/loggers/EventLogger.h
        src/loggers/EventLogger.cpp
        src/loggers/ConsoleEventLogger.h
        src/loggers/ConsoleEventLogger.cpp
        src/model_loaders/Model3D.h
        src/model_loaders/Model3D.cpp
        src/model_loaders/MD2Model.h
        src/model_loaders/MD2Model.cpp
        src/model_loaders/MD2Loader.h
        src/model_loaders/MD2Loader.cpp
        )

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(viking_engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/)