#pragma once

#include "../physics.h"
#include <BulletCollision/Gimpact/btGImpactShape.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

//std
#include <memory>
#include <variant>

namespace ray_sim
{

    using CollisionShapeVariant = std::variant<
        btBoxShape,
        btSphereShape,
        btCapsuleShape,
        btCylinderShape,
        btConeShape,
        btStaticPlaneShape,
        btCompoundShape,
        btConvexHullShape,
        btConvexTriangleMeshShape,
        btBvhTriangleMeshShape,
        btHeightfieldTerrainShape,
        btGImpactMeshShape,
        btMultiSphereShape,
        btUniformScalingShape,
        btEmptyShape
    >;

    struct PhysicsBody
    {
        PhysicsBody(PhysicsWorld& physicsWorld, btScalar mass, const btVector3& inertia, const btTransform& transform, const CollisionShapeVariant& shape);
        ~PhysicsBody();

        [[nodiscard]] u32 getRigidBodyId() const noexcept;
        [[nodiscard]] const Mesh& getCollisionMesh() const noexcept;
        [[nodiscard]] Mesh convertCollisionShapeToMesh() const;

    private:
        btScalar mass;
        std::unique_ptr<btMotionState> motionState;
        std::unique_ptr<btCollisionShape> collisionShape;
        PhysicsWorld &physicsWorld_;
        u32 rigidBodyId_;
        Mesh mesh_;
    };

    inline PhysicsBody::PhysicsBody(PhysicsWorld &physicsWorld, btScalar mass, const btVector3& inertia, const btTransform& transform, const CollisionShapeVariant& shape)
        : physicsWorld_{physicsWorld}
    , mass{mass}
    , collisionShape{nullptr}
    , motionState{std::make_unique<btDefaultMotionState>(transform)}
    , rigidBodyId_{0}
    , mesh_{}
    {
        // Use std::visit to handle different shapes
        std::visit([this](const auto& specificShape) {
            using ShapeType = std::decay_t<decltype(specificShape)>;
            this->collisionShape = std::make_unique<ShapeType>(specificShape);
        }, shape);

        mesh_ = convertCollisionShapeToMesh();

        btVector3 localInertia(0, 0, 0);

        if (auto boxShape = dynamic_cast<btBoxShape *>(collisionShape.get())) {
            if (mass != 0.0f) {
                boxShape->calculateLocalInertia(mass, localInertia); // Calculate inertia for the box shape
            }
        }

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
        if (auto boxShape = dynamic_cast<btBoxShape *>(collisionShape.get())) {
            // Handle box shape
            btVector3 halfExtents = boxShape->getHalfExtentsWithMargin();
            mesh = GenMeshCube(halfExtents.getX() * 2, halfExtents.getY() * 2, halfExtents.getZ() * 2);
        } else if (auto sphereShape = dynamic_cast<btSphereShape *>(collisionShape.get())) {
            // Handle sphere shape
            float radius = sphereShape->getRadius();
            mesh = GenMeshSphere(radius, 16, 16);
        } else if (auto cylinderShape = dynamic_cast<btCylinderShape *>(collisionShape.get())) {
            // Handle cylinder shape
            btVector3 halfExtents = cylinderShape->getHalfExtentsWithMargin();
            mesh = GenMeshCylinder(halfExtents.getX(), halfExtents.getY() * 2, 16);
        } else if (auto capsuleShape = dynamic_cast<btCapsuleShape *>(collisionShape.get())) {
            // Handle capsule shape
            float radius = capsuleShape->getRadius();
            float height = capsuleShape->getHalfHeight() * 2;
            mesh = GenMeshCylinder(radius, height, 16);
        } else {
            // Other shapes (e.g., btConvexHullShape, btCompoundShape) require custom handling
            TraceLog(LOG_WARNING, "Unsupported shape type");
        }
        UploadMesh(&mesh, false); // Upload mesh to GPU for rendering
        return mesh;
    }
}
