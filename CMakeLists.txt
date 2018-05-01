cmake_minimum_required(VERSION 3.10)
project(viking_engine)

set(CMAKE_CXX_STANDARD 11)

#file(GLOB_RECURSE sources **/*.cpp **/*.h)
#add_executable(viking_engine ${sources})
add_executable(viking_engine src/Main.cpp
        src/Camara.h
        src/Camara.cpp
        src/VKSkyDome.h
        src/VKSkyDome.cpp
        src/framework/sky/SkyBox.h
        src/framework/sky/SkyBox.cpp
        src/framework/Fuente.h
        src/framework/Fuente.cpp
        src/framework/Timer.h
        src/framework/Timer.cpp
        src/framework/Textura.h
        src/framework/Textura.cpp
        src/framework/BMPWriter.h
        src/framework/BMPWriter.cpp
        src/framework/sky/SkyDome.h
        src/framework/sky/SkyDome.cpp
        src/framework/sky/TexturedSkyDome.h
        src/framework/sky/TexturedSkyDome.cpp
        src/framework/sky/Clouds.h
        src/framework/sky/Clouds.cpp
        src/framework/sky/CloudsActualizables.h
        src/framework/sky/CloudsActualizables.cpp
        src/framework/sky/TexturedSkyPlane.h
        src/framework/sky/TexturedSkyPlane.cpp
        src/framework/TextureWriter.h
        src/framework/TextureWriter.cpp
        src/framework/Billboard.h
        src/framework/Billboard.cpp
        src/framework/loggers/EventLogger.h
        src/framework/loggers/EventLogger.cpp
        src/framework/loggers/ConsoleEventLogger.h
        src/framework/loggers/ConsoleEventLogger.cpp
        src/framework/model_loaders/Model3d.h
        src/framework/model_loaders/Model3d.cpp
        src/framework/model_loaders/Md2Model.h
        src/framework/model_loaders/Md2Model.cpp
        src/framework/model_loaders/MD2Loader.h
        src/framework/model_loaders/MD2Loader.cpp
        src/framework/model_loaders/ModelFactory.cpp src/framework/model_loaders/ModelFactory.h)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

target_link_libraries(viking_engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/)