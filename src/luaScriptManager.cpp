#include "luaScriptManager.h"

//std
#include <algorithm>
#include <iostream>

LuaScriptManager::LuaScriptManager()
{
    L = luaL_newstate();

    if (L == nullptr) {
        std::cout << "Failed to initialize Lua." << std::endl;
        return;
    }

    luaL_openlibs(L);
}

LuaScriptManager::~LuaScriptManager()
{
    lua_close(L);
}

bool LuaScriptManager::loadLuaScript(const std::filesystem::path &path)
{
    if (!std::filesystem::exists(path))
    {
        std::cout << "Script " << path << " does not exists.\n";
        return false;
    }

    if (luaL_dofile(L, path.string().c_str()) != LUA_OK)
    {
        std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        return false;
    }

    lua_getglobal(L, "update");
    lua_pushnumber(L, 98);
    lua_pcall(L, 1, 1, 0);

    std::cout <<  "RETURN LUA: "  << lua_toboolean(L, -1) << std::endl;

    luaScripts_.emplace_back(path);
    return true;
}

bool LuaScriptManager::callUpdateFunction(f32 dt)
{
    for (const auto &script: luaScripts_)
    {
        if (luaL_dofile(L, script.string().c_str()) != LUA_OK)
        {
            std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1);
            return false;
        }
    }
}
