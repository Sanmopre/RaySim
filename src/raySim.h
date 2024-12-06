#pragma once

#include "physics.h"
#include "textureComponent.h"

// raylib
#include "raylib.h"

// entt
#include "entt.hpp"


namespace ray_engine
{

enum ApplicationState
{
    RUNNING,
    USER_EXIT,
    ERROR_EXIT
};

class RaySim
{
public:
    RaySim();
    ~RaySim();

public:
    [[nodiscard]] ApplicationState Init();
    [[nodiscard]] ApplicationState Update();
    void Shutdown();
    entt::entity createEntity();

private:
    void updatePhysics();
    void drawTextures();

private:
    ray_engine::Physics physics_;
    Camera2D camera_;
    const int targetFps;
    const int screenWidth_;
    const int screenHeight_;
    entt::registry registry_;
};

}