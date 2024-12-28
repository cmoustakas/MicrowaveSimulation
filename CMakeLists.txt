cmake_minimum_required(VERSION 3.10)
# Project name and version
project(MicrowaveSimulation VERSION 1.0 LANGUAGES CXX CUDA)
set(CMAKE_CUDA_ARCHITECTURES "native")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(OpenGL_GL_PREFERENCE GLVND)

# Specify the source files

set(SOURCES
    src/main.cpp
    src/ModelImport.cpp
    include/ModelImport.hpp
    src/Mesh.cpp
    include/Mesh.hpp
    src/GraphicsUtils.cpp
    include/GraphicsUtils.hpp
    src/WindowHandler.cpp
    include/WindowHandler.hpp
    src/Engine.cpp
    include/Engine.hpp
    src/CameraHandler.cpp
    include/CameraHandler.hpp
    src/Demo.cpp
    include/Demo.hpp
    src/shaders/vertex.glsl
    src/shaders/fragment.glsl)

# Add an executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Find and link OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

# Find and link GLFW
find_package(glfw3 REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw)

# Find and link GLEW
find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)

# Find and link GLM (header-only, no linking needed)
find_path(GLM_INCLUDE_DIR glm/glm.hpp)
include_directories(${GLM_INCLUDE_DIR})


# Find and link Assimp
find_package(assimp REQUIRED)
target_link_libraries(${PROJECT_NAME} assimp)

# Find and link CUDA
find_package(CUDAToolkit REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${CUDAToolkit_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME})
