#include "physics.h"

#include "types.h"

ray_engine::Physics::Physics()
{
    worldDef_ = b2DefaultWorldDef();
    worldDef_.gravity = b2Vec2{0.0f, -9.81f};
    worldId_ = b2CreateWorld(&worldDef_);
}

ray_engine::Physics::~Physics()
{
    for (const auto& bodyId : bodiesVector_)
    {
        b2DestroyBody(bodyId);
    }

    b2DestroyWorld(worldId_);
}

b2BodyId ray_engine::Physics::createPhysicsObject(const b2BodyDef& bodyDef,const b2Polygon& polygon,const b2ShapeDef& shapeDef)
{
    b2BodyId bodyId = b2CreateBody(worldId_, &bodyDef);
    b2CreatePolygonShape(bodyId, &shapeDef, &polygon);
    bodiesVector_.emplace_back(bodyId);
    return bodyId;
}

ray_engine::Transform ray_engine::Physics::getTransform(b2BodyId bodyId)
{
    Transform transform;
    transform.position = b2Body_GetPosition(bodyId);
    transform.rotation = b2Body_GetRotation(bodyId);
    return transform;
}

void ray_engine::Physics::update() const
{
    constexpr f32 timeStep = 1.0f / 60.0f;
    constexpr u16 subStepCount = 4;
    b2World_Step(worldId_, timeStep, subStepCount);
}
