#pragma once

#include <filesystem>

class LuaScriptManager
{
public:
    LuaScriptManager();
    ~LuaScriptManager();

public:
    bool LoadLuaScript(const std::filesystem::path& path);

};

