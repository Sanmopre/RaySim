#pragma once

#include "../physics.h"

// bullet
#include <BulletCollision/Gimpact/btGImpactShape.h>
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

//std
#include <memory>




namespace ray_sim
{
    enum class CollisionType
    {
        BOX,
        SPHERE,
        MESH,
        TERRAIN
    };

    struct PhysicsBody
    {
        PhysicsBody(PhysicsWorld& physicsWorld, btScalar mass, const btTransform& transform, const btBoxShape& shape);
        PhysicsBody(PhysicsWorld& physicsWorld, btScalar mass, const btTransform& transform, const btSphereShape& shape);
        PhysicsBody(PhysicsWorld &physicsWorld, btScalar mass, const btTransform &transform, const btTriangleMeshShape& shape);
        PhysicsBody(PhysicsWorld &physicsWorld, btScalar mass, const btTransform &transform, const btHeightfieldTerrainShape& shape);

        ~PhysicsBody();

        [[nodiscard]] u32 getRigidBodyId() const noexcept;
        [[nodiscard]] const Mesh& getCollisionMesh() const noexcept;

    private:
        [[nodiscard]] Mesh convertCollisionShapeToMesh() const;

    private:
        const CollisionType collisionType_;
        btScalar mass;
        std::unique_ptr<btMotionState> motionState;
        std::shared_ptr<btCollisionShape> collisionShape;
        PhysicsWorld &physicsWorld_;
        u32 rigidBodyId_;
        Mesh mesh_;
    };

    inline PhysicsBody::PhysicsBody(PhysicsWorld &physicsWorld, btScalar mass, const btTransform &transform,
    const btBoxShape &shape)
    : collisionType_{CollisionType::BOX}
    , mass{mass}
    , motionState{std::make_unique<btDefaultMotionState>(transform)}
    , collisionShape{std::make_shared<btBoxShape>(shape)}
    , physicsWorld_{physicsWorld}
    , rigidBodyId_{0}
    , mesh_{convertCollisionShapeToMesh()}
    {
        btVector3 localInertia(0, 0, 0);
        if (mass != 0.0f) {
            shape.calculateLocalInertia(mass, localInertia);
        }

        // Create the rigid body
        rigidBodyId_ = physicsWorld_.createNewRigidBody(btRigidBody::btRigidBodyConstructionInfo(
            mass,
            motionState.get(),
            collisionShape.get(),
            localInertia
        ));
    }

    inline PhysicsBody::PhysicsBody(PhysicsWorld &physicsWorld, btScalar mass, const btTransform &transform,
        const btSphereShape &shape)
    : collisionType_{CollisionType::SPHERE}
    , mass{mass}
    , motionState{std::make_unique<btDefaultMotionState>(transform)}
    , collisionShape{std::make_shared<btSphereShape>(shape)}
    , physicsWorld_{physicsWorld}
    , rigidBodyId_{0}
    , mesh_{convertCollisionShapeToMesh()}
    {
        btVector3 localInertia(0, 0, 0);
        if (mass != 0.0f) {
            shape.calculateLocalInertia(mass, localInertia);
        }

        // Create the rigid body
        rigidBodyId_ = physicsWorld_.createNewRigidBody(btRigidBody::btRigidBodyConstructionInfo(
            mass,
            motionState.get(),
            collisionShape.get(),
            localInertia
        ));
    }

    inline PhysicsBody::PhysicsBody(PhysicsWorld &physicsWorld, btScalar mass, const btTransform &transform,
            const btTriangleMeshShape& shape)
        : collisionType_{CollisionType::MESH}
    , mass{mass}
    , motionState{std::make_unique<btDefaultMotionState>(transform)}
    , collisionShape{std::make_shared<btTriangleMeshShape>(shape)}  // Store shared pointer
    , physicsWorld_{physicsWorld}
    , rigidBodyId_{0}
    , mesh_{convertCollisionShapeToMesh()}
    {
        btVector3 localInertia(0, 0, 0);
        if (mass != 0.0f) {
            shape.calculateLocalInertia(mass, localInertia);
        }

        collisionShape->setMargin(0.01f);

        // Create the rigid body
        rigidBodyId_ = physicsWorld_.createNewRigidBody(btRigidBody::btRigidBodyConstructionInfo(
            mass,
            motionState.get(),
            collisionShape.get(),
            localInertia
        ));
    }

    inline PhysicsBody::PhysicsBody(PhysicsWorld &physicsWorld, btScalar mass, const btTransform &transform,
        const btHeightfieldTerrainShape &shape)
    : collisionType_{CollisionType::TERRAIN}
    , mass{mass}
    , motionState{std::make_unique<btDefaultMotionState>(transform)}
    , collisionShape{std::make_shared<btHeightfieldTerrainShape>(shape)}
    , physicsWorld_{physicsWorld}
    , rigidBodyId_{0}
    , mesh_{convertCollisionShapeToMesh()}
    {
        btVector3 localInertia(0, 0, 0);
        if (mass != 0.0f) {
            shape.calculateLocalInertia(mass, localInertia);
        }

        collisionShape->setMargin(0.01f);

        // Create the rigid body
        rigidBodyId_ = physicsWorld_.createNewRigidBody(btRigidBody::btRigidBodyConstructionInfo(
            mass,
            motionState.get(),
            collisionShape.get(),
            localInertia
        ));
    }

    inline PhysicsBody::~PhysicsBody()
    {
        physicsWorld_.deleteRigidBody(rigidBodyId_);
    }

    inline u32 PhysicsBody::getRigidBodyId() const noexcept
    {
        return rigidBodyId_;
    }

    inline const Mesh & PhysicsBody::getCollisionMesh() const noexcept
    {
        return mesh_;
    }

    inline Mesh PhysicsBody::convertCollisionShapeToMesh() const
    {
        Mesh mesh = { 0 };

        switch (collisionType_)
        {
            case CollisionType::BOX:
                {
                    auto boxShape = dynamic_cast<btBoxShape*>(collisionShape.get());
                    const btVector3 halfExtents = boxShape->getHalfExtentsWithMargin();
                    mesh = GenMeshCube(halfExtents.getX() * 2, halfExtents.getY() * 2, halfExtents.getZ() * 2);
                    UploadMesh(&mesh, false);
                }
                break;
            case CollisionType::SPHERE:
                {
                    mesh = GenMeshSphere(dynamic_cast<btSphereShape *>(collisionShape.get())->getRadius(), 16, 16);
                    UploadMesh(&mesh, false);
                }
                break;
            case CollisionType::MESH:
                break;
            case CollisionType::TERRAIN:
                break;
            default:
                break;
        }

        return mesh;
    }
}
