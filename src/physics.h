#pragma once

// box2d
#include <vector>

#include "box2d/box2d.h"

namespace ray_engine
{
class Physics
{
public:

    Physics();
    ~Physics();

    b2BodyId createPhysicsObject(const b2BodyDef& bodyDef,const b2Polygon& polygon,const b2ShapeDef& shapeDef);

    void update() const;

private:
    b2WorldDef worldDef_;
    b2WorldId worldId_;
    std::vector<b2BodyId> bodiesVector_;
};
}