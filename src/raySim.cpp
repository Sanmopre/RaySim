#include "raySim.h"

#include <iostream>

#include "raymath.h"
#include "rlgl.h"
#include "components/transformComponent.h"
#include "components/physicsBodyComponent.h"
#include "components/meshComponent.h"

namespace ray_sim
{

namespace
{
    btQuaternion fromEuler(float roll, float pitch, float yaw)
    {
        const auto quat = QuaternionFromEuler(pitch, yaw, roll);
        return btQuaternion(quat.x, quat.y, quat.z, quat.w).normalize();
    }
}

RaySim::RaySim()
:
physicsWorld_{btVector3(static_cast<btScalar>(0.0), static_cast<btScalar>(-9.81), static_cast<btScalar>(0.0))}
, camera_{0}
, targetFps{60}
, screenWidth_{1920}
, screenHeight_{1080}
{
}

RaySim::~RaySim()
{
}

ApplicationState RaySim::Init()
{
    InitWindow(screenWidth_, screenHeight_, "RaySim");

    entity1 = registry_.create();
    entity2 = registry_.create();

    auto& entity1Transform = registry_.emplace<ray_sim::Transform>(entity1);
    entity1Transform.transform.setOrigin({0, 50, 0});
    entity1Transform.transform.setRotation(fromEuler(0, 0,0));
    const auto& physicsBody = registry_.emplace<PhysicsBody>(entity1, physicsWorld_, 1.0f, btVector3{}, entity1Transform.transform, btBoxShape(btVector3(1, 1, 1)));
    auto& simModel = registry_.emplace<SimModel>(entity1);
    simModel.loadModelFromMesh(physicsBody.getCollisionMesh());
    simModel.setColor(WHITE);


    auto& entity2Transform = registry_.emplace<ray_sim::Transform>(entity2);
    entity2Transform.transform.setOrigin({0, 0, 0});
    entity2Transform.transform.setRotation(fromEuler(0, 45 *DEG2RAD,0));
    const auto& physicsBody2 = registry_.emplace<PhysicsBody>(entity2, physicsWorld_, 0.0f, btVector3{}, entity2Transform.transform, btBoxShape(btVector3(10, 1, 10)));
    auto& simModel2 = registry_.emplace<SimModel>(entity2);
    simModel2.loadModelFromMesh(physicsBody2.getCollisionMesh());
    simModel2.setColor(WHITE);

    camera_.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera_.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera_.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera_.fovy = 45.0f;                                // Camera field-of-view Y
    camera_.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    DisableCursor();

    SetTargetFPS(targetFps);

    texture = LoadTexture("../resources/wood.png");
    shader = LoadShader("../resources/base_lighting.vs", "../resources/lighting.fs");

    // Set light properties
    Vector3 lightPosition = (Vector3){ 0.0f, 2.0f, 2.0f };
    int lightLoc = GetShaderLocation(shader, "lightPosition");
    SetShaderValue(shader, lightLoc, &lightPosition, SHADER_UNIFORM_VEC3);

    return ApplicationState::RUNNING;
}

ApplicationState RaySim::Update()
{
    if (WindowShouldClose())
    {
        return ApplicationState::USER_EXIT;
    }

    constexpr f32 deltaTime = 1.0f / static_cast<f32>(60);
    physicsWorld_.step(deltaTime, 10, 1.0f / 240.0f);

    UpdateCamera(&camera_, CAMERA_FREE);

    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera_);

    auto renderEntitiesView = registry_.view<ray_sim::Transform, ray_sim::PhysicsBody, ray_sim::SimModel>();
    for(const auto& entity : registry_.view<ray_sim::Transform, ray_sim::PhysicsBody>())
    {
        auto& transform = renderEntitiesView.get<ray_sim::Transform>(entity).transform;
        auto& body = renderEntitiesView.get<ray_sim::PhysicsBody>(entity);
        auto& simModel = renderEntitiesView.get<ray_sim::SimModel>(entity);

        physicsWorld_.getRigidBody(body.getRigidBodyId())->getMotionState()->getWorldTransform(transform);

        // Create a Model from the Mesh
        Model& model = simModel.getModel();

        // Extract the quaternion directly from the Bullet transform
        btQuaternion rotation = transform.getRotation();

        // Convert the Bullet quaternion to a raylib Matrix
        Matrix rotationMatrix = {
            1 - 2 * rotation.y() * rotation.y() - 2 * rotation.z() * rotation.z(),
            2 * rotation.x() * rotation.y() - 2 * rotation.z() * rotation.w(),
            2 * rotation.x() * rotation.z() + 2 * rotation.y() * rotation.w(),
            0,

            2 * rotation.x() * rotation.y() + 2 * rotation.z() * rotation.w(),
            1 - 2 * rotation.x() * rotation.x() - 2 * rotation.z() * rotation.z(),
            2 * rotation.y() * rotation.z() - 2 * rotation.x() * rotation.w(),
            0,

            2 * rotation.x() * rotation.z() - 2 * rotation.y() * rotation.w(),
            2 * rotation.y() * rotation.z() + 2 * rotation.x() * rotation.w(),
            1 - 2 * rotation.x() * rotation.x() - 2 * rotation.y() * rotation.y(),
            0,

            0, 0, 0, 1
        };

        // Set the model's transform using the rotation matrix
        model.transform = MatrixMultiply(rotationMatrix, MatrixTranslate(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ()));
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
        model.materials[0].shader = shader;
        DrawModel(model, Vector3Zero(), 1.0f, simModel.getColor());
        //drawModelWireFrame(model);
    }

    DrawGrid(100, 1.0f);

    EndMode3D();

    EndDrawing();

    return ApplicationState::RUNNING;
}

void RaySim::Shutdown()
{
    registry_.clear();
    CloseWindow();
}

void RaySim::drawModelWireFrame(const Model &model) const
{
    // Draw the wireframe overlay
    rlPushMatrix();
    rlMultMatrixf(MatrixToFloat(model.transform)); // Apply model transformation
    for (int i = 0; i < model.meshCount; i++) {
        rlEnableWireMode(); // Enable wireframe mode
        DrawMesh(model.meshes[i], model.materials[0], MatrixIdentity()); // Draw the mesh as wireframe
        rlDisableWireMode(); // Disable wireframe mode
    }
    rlPopMatrix();
}
}
