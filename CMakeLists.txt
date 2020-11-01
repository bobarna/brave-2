cmake_minimum_required(VERSION 3.1)

project(brave2)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/utils/math.h src/utils/math.cpp
        src/utils/util.h src/utils/util.cpp
        src/utils/gl_helper.h
        src/Particle.h
        src/HairSimulation.h src/HairSimulation.cpp
        src/PBD_Simulation.h src/PBD_Simulation.cpp
        src/utils/other/stb_image_write.h
        src/utils/save_image.h
        src/Scene.cpp src/Scene.h
        src/Camera.cpp src/Camera.h
        src/RenderState.h
        src/Geometry.cpp src/Geometry.h
        src/Object.cpp src/Object.h
        src/Shader.cpp src/Shader.h
        src/MaterialAndTexture.cpp src/MaterialAndTexture.h
        src/Light.h)

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

if (WIN32)
    # nothing now
endif (WIN32)

target_link_libraries(
        ${PROJECT_NAME}
        glfw
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
)
