#pragma once

#include <filesystem>
#include <vector>

// lua
#include "lua.hpp"
#include "lauxlib.h"
#include "lualib.h"
#include "types.h"


class LuaScriptManager
{
public:
    LuaScriptManager();
    ~LuaScriptManager();

public:
    [[nodiscard]] bool loadLuaScript(const std::filesystem::path& path);
    [[nodiscard]] bool callUpdateFunction(f32 dt);

private:
    lua_State* L;
    std::vector<std::filesystem::path> luaScripts_;
};

