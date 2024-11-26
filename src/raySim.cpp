#include "raySim.h"
#include "types.h"

namespace ray_sim
{

RaySim::RaySim()
:
  camera_{0}
, targetFps{60}
, screenWidth_{1920}
, screenHeight_{1080}
{
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
