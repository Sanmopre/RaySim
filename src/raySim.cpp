#include "raySim.h"
#include "types.h"

#include <iostream>

namespace ray_sim
{

RaySim::RaySim()
:
  camera_{0}
, targetFps{60}
, screenWidth_{1920}
, screenHeight_{1080}
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = {0, 10};
    bodyDef.type = b2_dynamicBody;

    b2Polygon polygon = b2MakeBox(0.5f, 0.5f);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    bodyId = physics_.createPhysicsObject(bodyDef, polygon, shapeDef);
}

RaySim::~RaySim()
{
}

ApplicationState RaySim::Init()
{
    InitWindow(screenWidth_, screenHeight_, "RaySim");


    Camera2D camera = { 0 };
    camera_.target = (Vector2){ 0, 0};
    camera_.offset = (Vector2){ static_cast<f32>(screenWidth_)/2.0f, static_cast<f32>(screenHeight_)/2.0f };
    camera_.rotation = 0.0f;
    camera_.zoom = 1.0f;
    DisableCursor();

    SetTargetFPS(targetFps);

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

    EndMode2D();

    DrawText("SCREEN AREA", 640, 10, 20, RED);

    EndDrawing();

    return ApplicationState::RUNNING;
}

void RaySim::Shutdown()
{
    registry_.clear();
    CloseWindow();
}

}
