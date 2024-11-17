cmake_minimum_required(VERSION 3.29)
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

project(raySim)

set(CMAKE_CXX_STANDARD 17)

# Add Raylib as a subdirectory
add_subdirectory(raylib-5.0)
add_subdirectory(bullet3)

add_executable(raySim src/main.cpp
        src/physics.cpp
        src/physics.h
        src/types.h
        src/idManager.h
        src/components/transformComponent.h
        src/raySim.cpp
        src/raySim.h
        src/components/physicsBodyComponent.h
        src/components/meshComponent.h
)

# Link Raylib and Bullet3 libraries to the executable
target_link_libraries(raySim raylib BulletDynamics BulletCollision LinearMath)

# Include Raylib and Bullet3 headers
target_include_directories(raySim PRIVATE raylib-5.0/src bullet3/src entt)