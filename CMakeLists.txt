cmake_minimum_required(VERSION 3.29)
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

project(raySim)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(thirdParty/raylib-5.0)
add_subdirectory(thirdParty/box2d-3.0.0)

add_executable(raySim
        src/main.cpp
        src/physics.cpp
        src/physics.h
        src/types.h
        src/idManager.h
        src/raySim.cpp
        src/raySim.h
        src/textureComponent.h
)

# Link Raylib and Bullet3 libraries to the executable
target_link_libraries(raySim raylib box2d)

# Include Raylib and Bullet3 headers
target_include_directories(raySim PRIVATE thirdParty/raylib-5.0/src thirdParty/entt thirdParty/fastNoiseLite thirdParty/box2d-3.0.0/include)