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
        src/objects/Particle.h
        src/objects/PBDSimulation.h src/objects/PBDSimulation.cpp
        src/utils/other/stb_image_write.h
        src/utils/save_image.h
        src/rendering/Shader.h
        src/objects/Geometry.h
        src/rendering/RenderState.h
        src/rendering/Camera.h src/rendering/Camera.cpp
        src/objects/Object.h src/objects/Object.cpp
        src/rendering/Scene.cpp src/rendering/Scene.h src/rendering/Shader.cpp src/Constants.h src/InputHandler.h src/InputHandler.cpp src/objects/Geometry.cpp)

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
