cmake_minimum_required(VERSION 3.29)
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

project(rayEngine)

set(CMAKE_CXX_STANDARD 17)

# Add Raylib, Box2D and Lua
add_subdirectory(thirdParty/raylib-5.0)
add_subdirectory(thirdParty/box2d-3.0.0)
add_subdirectory(thirdParty/lua-5.4.7)

# Create rayEngine executable and include Lua sources
add_executable(rayEngine
        src/main.cpp
        src/physics.cpp
        src/physics.h
        src/types.h
        src/idManager.h
        src/raySim.cpp
        src/raySim.h
        src/textureComponent.h
        src/luaScriptManager.cpp
        src/luaScriptManager.h
)

# Include Lua headers
target_include_directories(rayEngine PRIVATE thirdParty/lua-5.4.7)

# Link libraries to rayEngine
target_link_libraries(rayEngine raylib box2d LuaLib)

# Include directories for other libraries
target_include_directories(rayEngine PRIVATE thirdParty/raylib-5.0/src thirdParty/entt thirdParty/fastNoiseLite thirdParty/box2d-3.0.0/include thirdParty/lua-5.4.7/src)