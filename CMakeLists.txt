cmake_minimum_required(VERSION 3.29)
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

project(rayEngine)

set(CMAKE_CXX_STANDARD 17)

# Add Raylib and Box2D
add_subdirectory(thirdParty/raylib-5.0)
add_subdirectory(thirdParty/box2d-3.0.0)

# Collect Lua source files
file(GLOB LUA_SOURCES thirdParty/lua-5.4.7/*.c)

# Exclude the main Lua interpreter file
list(REMOVE_ITEM LUA_SOURCES ${CMAKE_SOURCE_DIR}/thirdParty/lua-5.4.7/lua.c)

# Add Lua library target
add_library(lua STATIC ${LUA_SOURCES})

# Include Lua headers
target_include_directories(lua PUBLIC thirdParty/lua-5.4.7)

# Create rayEngine executable
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

# Link libraries to rayEngine
target_link_libraries(rayEngine lua raylib box2d)

# Include directories
target_include_directories(rayEngine PRIVATE thirdParty/raylib-5.0/src thirdParty/entt thirdParty/fastNoiseLite thirdParty/box2d-3.0.0/include thirdParty/lua-5.4.7)