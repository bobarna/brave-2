cmake_minimum_required(VERSION 3.1)

project(brave2)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/utils/math.h src/utils/math.cpp
        src/utils/util.h src/utils/util.cpp
        src/geometries/PBDSimulation.h src/geometries/PBDSimulation.cpp
        src/rendering/Camera.h src/rendering/Camera.cpp
        src/objects/Object.h src/objects/Object.cpp
        src/rendering/Scene.cpp
        src/rendering/shaders/Shader.cpp
        src/utils/InputHandler.cpp
        src/geometries/Geometry.cpp
        src/objects/HairSimObject.cpp
        src/objects/HeadObject.cpp
        src/rendering/shaders/BasicShader.cpp src/rendering/shaders/BasicShader.h
        src/rendering/shaders/PhongShader.cpp src/rendering/shaders/PhongShader.h
        src/geometries/ParamSurface.cpp src/geometries/ParamSurface.h
        src/geometries/VertexData.h src/rendering/materials/Material.cpp src/rendering/materials/Material.h src/rendering/textures/Texture.cpp src/rendering/textures/Texture.h src/rendering/lights/Light.cpp src/rendering/lights/Light.h src/geometries/VertexData.cpp src/utils/OBJReader.cpp src/utils/OBJReader.h src/geometries/ObjGeometry.cpp src/geometries/ObjGeometry.h)

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# glfw
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

target_link_libraries(
        ${PROJECT_NAME}
        glfw
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
)
