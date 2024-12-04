#include "raySim.h"
#include "types.h"

#include <iostream>

namespace ray_sim
{

RaySim::RaySim()
:
  camera_{0}
, targetFps{60}
, screenWidth_{800}
, screenHeight_{800}
{

    const auto id = registry_.create();

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = {0, 10};
    bodyDef.type = b2_dynamicBody;

    b2Polygon polygon = b2MakeBox(0.5f, 0.5f);
    b2ShapeDef shapeDef = b2DefaultShapeDef();

    bodyId = registry_.emplace<b2BodyId>(id, physics_.createPhysicsObject(bodyDef, polygon, shapeDef));



    b2BodyDef bodyDef2 = b2DefaultBodyDef();
    bodyDef2.position = {0, 0};
    bodyDef2.type = b2_staticBody;

    b2Polygon polygon2 = b2MakeBox(20.0f, 0.5f);
    b2ShapeDef shapeDef2 = b2DefaultShapeDef();
    terrainId = physics_.createPhysicsObject(bodyDef2, polygon2, shapeDef2);
}

RaySim::~RaySim()
{
}

ApplicationState RaySim::Init() {
    InitWindow(screenWidth_, screenHeight_, "RaySim");


    Camera2D camera = { 0 };
    camera_.target = (Vector2){ 0, 0};
    camera_.offset = (Vector2){ static_cast<f32>(screenWidth_)/2.0f, static_cast<f32>(screenHeight_)/2.0f };
    camera_.rotation = 0.0f;
    camera_.zoom = 1.0f;
    DisableCursor();

    SetTargetFPS(targetFps);

    textureComponent_.addTexture("../resources/wood.png",{0.0f,0.0f},{10.0f,10.0f});

    return ApplicationState::RUNNING;
}

ApplicationState RaySim::Update()
{
    if (WindowShouldClose())
    {
        return ApplicationState::USER_EXIT;
    }
    physics_.update();
    std::cout << physics_.getTransform(bodyId).position.y << std::endl;

    BeginDrawing();
    BeginMode2D(camera_);

    ClearBackground(DARKBLUE);

    for (const auto& textureData : textureComponent_.getTexturesData())
    {
        DrawTexture(textureData.texture, textureData.relativePosition.x, textureData.relativePosition.y, WHITE);
    }

    EndMode2D();
    EndDrawing();

    return ApplicationState::RUNNING;
}

void RaySim::Shutdown()
{
    registry_.clear();
    CloseWindow();
}

}
