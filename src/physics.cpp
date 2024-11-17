//
// Created by sanmopre on 11/9/24.
//

#include "physics.h"

PhysicsWorld::PhysicsWorld(btVector3 const &gravity):
collisionConfiguration_{std::make_unique<btDefaultCollisionConfiguration>()},
dispatcher_{std::make_unique<btCollisionDispatcher>(collisionConfiguration_.get())},
overlappingPairCache_{std::make_unique<btDbvtBroadphase>()},
solver_{std::make_unique<btSequentialImpulseConstraintSolver>()},
discreteDynamicsWorld_ {std::make_unique<btDiscreteDynamicsWorld>(dispatcher_.get(), overlappingPairCache_.get(), solver_.get(), collisionConfiguration_.get())}
{
    discreteDynamicsWorld_->setGravity(gravity);
}

btDiscreteDynamicsWorld* PhysicsWorld::getWorld()
{
    return discreteDynamicsWorld_.get();
}

void PhysicsWorld::step(btScalar timeStep, int maxSubSteps, btScalar fixedTimeStep)
{
    discreteDynamicsWorld_->stepSimulation(timeStep,maxSubSteps,fixedTimeStep);
}

u32 PhysicsWorld::createNewRigidBody(const btRigidBody::btRigidBodyConstructionInfo &constructionInfo)
{
    const u32 id = idManager_.createId();

    rigidBodyMap_.try_emplace(id, std::make_unique<btRigidBody>(constructionInfo));
    discreteDynamicsWorld_->addRigidBody(rigidBodyMap_[id].get());
    return id;
}

void PhysicsWorld::deleteRigidBody(const u32 rigidBodyId)
{
    if (const auto it = rigidBodyMap_.find(rigidBodyId); it != rigidBodyMap_.end())
    {
        discreteDynamicsWorld_->removeRigidBody(it->second.get());
        rigidBodyMap_.erase(it);
        idManager_.releaseId(rigidBodyId);
    }
}

btRigidBody* PhysicsWorld::getRigidBody(const u32 rigidBodyId) const
{
    return rigidBodyMap_.at(rigidBodyId).get();
}

