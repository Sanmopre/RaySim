#include "raySim.h"
#include "types.h"

#include <iostream>

namespace ray_engine
{

RaySim::RaySim()
:
  camera_{0}
, targetFps{60}
, screenWidth_{800}
, screenHeight_{800}
{
}

RaySim::~RaySim()
{
}

ApplicationState RaySim::Init()
{
    InitWindow(screenWidth_, screenHeight_, "RaySim");

    const auto id = createEntity();

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = {0, 10};
    bodyDef.type = b2_dynamicBody;

    b2Polygon polygon = b2MakeBox(0.5f, 0.5f);
    b2ShapeDef shapeDef = b2DefaultShapeDef();

    std::ignore = registry_.emplace<b2BodyId>(id, physics_.createPhysicsObject(bodyDef, polygon, shapeDef));
    auto& textureComp = registry_.emplace<TextureComponent>(id);
    textureComp.addTexture("../resources/wood.png",{0.0f,0.0f},{1.0f,1.0f});

    b2BodyDef bodyDef2 = b2DefaultBodyDef();
    bodyDef2.position = {0, 0};
    bodyDef2.type = b2_staticBody;

    b2Polygon polygon2 = b2MakeBox(20.0f, 0.5f);
    b2ShapeDef shapeDef2 = b2DefaultShapeDef();
    terrainId = physics_.createPhysicsObject(bodyDef2, polygon2, shapeDef2);

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

    updatePhysics();
    drawTextures();

    return ApplicationState::RUNNING;
}

void RaySim::Shutdown()
{
    registry_.clear();
    CloseWindow();
}

entt::entity RaySim::createEntity()
{
    const auto entityId = registry_.create();
    registry_.emplace<ray_engine::Transform>(entityId);
    return entityId;
}

void RaySim::updatePhysics()
{
    physics_.update();

    // Update the transform of all entities with a bodyId
    const auto physicsEntitiesView = registry_.view<ray_engine::Transform, b2BodyId>();
    for (const auto& physicsEntity : physicsEntitiesView)
    {
        auto& transform = registry_.get<ray_engine::Transform>(physicsEntity);
        transform = physics_.getTransform(registry_.get<b2BodyId>(physicsEntity));
    }
}

void RaySim::drawTextures()
{
    BeginDrawing();
    BeginMode2D(camera_);

    ClearBackground(DARKBLUE);

    // Update the transform of all entities with a bodyId
    const auto textureEntitiesView = registry_.view<ray_engine::Transform, TextureComponent>();

    for (const auto& textureEntity : textureEntitiesView)
    {
        const auto& transform = registry_.get<ray_engine::Transform>(textureEntity);
        const auto& textureComp = registry_.get<TextureComponent>(textureEntity);

        for (const auto& textureData : textureComp.getTexturesData())
        {
            const int x = static_cast<int>(transform.position.x + textureData.relativePosition.x);
            const int y = static_cast<int>(transform.position.y + textureData.relativePosition.y);
            DrawTexture(textureData.texture, x, y, WHITE);
        }
    }

    EndMode2D();
    EndDrawing();
}

}
