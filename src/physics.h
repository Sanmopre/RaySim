#pragma once

#include "types.h"

// box2d
#include "box2d/box2d.h"

//std
#include <vector>

#define PHYSICS_TO_PIXELS(x) (f32)(x/2.0f)

namespace ray_engine
{

struct Transform
{
    b2Vec2 position = { 0, 0 };
    b2Rot rotation = b2Rot();
};

class Physics
{
public:

    Physics();
    ~Physics();

    b2BodyId createPhysicsObject(const b2BodyDef& bodyDef,const b2Polygon& polygon,const b2ShapeDef& shapeDef);

    Transform getTransform(b2BodyId bodyId);

    void update() const;

private:
    b2WorldDef worldDef_;
    b2WorldId worldId_;
    std::vector<b2BodyId> bodiesVector_;
};
}