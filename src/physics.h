#pragma once

// std
#include <memory>
#include <map>

// bullet
#include "btBulletDynamicsCommon.h"

#include "types.h"
#include "idManager.h"


class PhysicsWorld final
{
public:
    PhysicsWorld(btVector3 const& gravity);
    ~PhysicsWorld() = default;

public:
    btDiscreteDynamicsWorld* getWorld();
    void step(btScalar timeStep, int maxSubSteps = 1, btScalar fixedTimeStep = btScalar(1.) / btScalar(60.));
    [[nodiscard]] u32 createNewRigidBody(const btRigidBody::btRigidBodyConstructionInfo& constructionInfo);
    void deleteRigidBody(const u32 rigidBodyId);

    [[nodiscard]] btRigidBody* getRigidBody(const u32 rigidBodyId) const;
private:

    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration_;
    std::unique_ptr<btCollisionDispatcher> dispatcher_;
    std::unique_ptr<btBroadphaseInterface> overlappingPairCache_;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver_;
    std::unique_ptr<btDiscreteDynamicsWorld> discreteDynamicsWorld_;
    std::map<u32, std::unique_ptr<btRigidBody>> rigidBodyMap_;
    IdManager idManager_;
};
