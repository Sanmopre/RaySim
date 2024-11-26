#include "physics.h"

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
