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
        src/PBDSimulation.h src/PBDSimulation.cpp
        src/utils/other/stb_image_write.h
        src/utils/save_image.h
        src/Shader.h
        src/Geometry.h
        src/RenderState.h
        src/Camera.h src/Camera.cpp
        src/Object.h src/Object.cpp
        src/Scene.cpp src/Scene.h src/Shader.cpp src/Constants.h src/InputHandler.h src/InputHandler.cpp)

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
