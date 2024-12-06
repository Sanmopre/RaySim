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
    bodyDef.position = {0, 100};
    bodyDef.rotation = b2MakeRot(50 * DEG2RAD);
    bodyDef.type = b2_dynamicBody;

    b2Polygon polygon = b2MakeBox(30.0f, 30.0f);
    b2ShapeDef shapeDef = b2DefaultShapeDef();

    std::ignore = registry_.emplace<b2BodyId>(id, physics_.createPhysicsObject(bodyDef, polygon, shapeDef));
    auto& textureComp = registry_.emplace<TextureComponent>(id);
    textureComp.addTexture("../resources/wood.png",{0.0f,0.0f},{30.0f,30.0f});


    const auto idTerrain = createEntity();
    b2BodyDef bodyDef2 = b2DefaultBodyDef();
    bodyDef2.position = {0, 0};
    bodyDef2.type = b2_staticBody;

    b2Polygon polygon2 = b2MakeBox(200.0f, 10.0f);
    b2ShapeDef shapeDef2 = b2DefaultShapeDef();
    std::ignore = registry_.emplace<b2BodyId>(idTerrain, physics_.createPhysicsObject(bodyDef2, polygon2, shapeDef2));
    auto& textureComp2 = registry_.emplace<TextureComponent>(idTerrain);
    textureComp2.addTexture("../resources/wood.png",{0.0f,0.0f},{200.0f, 10.0f});

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

    ClearBackground(RAYWHITE);

    // Update the transform of all entities with a bodyId
    const auto textureEntitiesView = registry_.view<ray_engine::Transform, TextureComponent>();

    for (const auto& textureEntity : textureEntitiesView)
    {
        const auto& transform = registry_.get<ray_engine::Transform>(textureEntity);
        const auto& textureComp = registry_.get<TextureComponent>(textureEntity);

        for (const auto& textureData : textureComp.getTexturesData())
        {
            const auto x = transform.position.x + textureData.relativePosition.x - textureData.texture.width/2.0f;

            // Y is negative since the raylib uses a  screen-space coordinate system instead of Cartesian coordinate system
            const auto y = -(transform.position.y + textureData.relativePosition.y - textureData.texture.height/2.0f);
            DrawTextureEx(textureData.texture, {x, y} , - b2Rot_GetAngle(transform.rotation) * RAD2DEG, 1.0f, WHITE);
        }
    }

    EndMode2D();
    EndDrawing();
}

}
