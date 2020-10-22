cmake_minimum_required(VERSION 3.1)

project(brave2)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")

add_executable(${PROJECT_NAME}
        src/main.cpp src/utils/math.h src/utils/gl_helper.h src/Shader.h src/Particle.h src/FTL_Simulation.cpp src/FTL_Simulation.h src/HairSimulation.cpp src/HairSimulation.h src/utils/util.h)

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
