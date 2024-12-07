#include "luaScriptManager.h"

//std
#include <algorithm>
#include <iostream>

// lua
#include "lauxlib.h"
#include "lualib.h"

LuaScriptManager::LuaScriptManager()
{
    // Initialize a Lua state
    lua_State* L = luaL_newstate();

    if (L == nullptr) {
        std::cout << "Failed to initialize Lua." << std::endl;
        return;
    }

    // Open Lua standard libraries
    luaL_openlibs(L);

    // Lua code as a string
    const char* luaCode = R"(
        print("Hello from Lua using a string!")
        local x = 10
        local y = 20
        print("Sum:", x + y)
    )";

    // Execute the Lua code string
    if (luaL_dostring(L, luaCode) != LUA_OK) {
        std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1); // Remove error message from stack
    }

    // Close the Lua state
    lua_close(L);
}

LuaScriptManager::~LuaScriptManager()
{
}
