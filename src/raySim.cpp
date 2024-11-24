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

    Matrix getRaylibRotationMatrixFromQuaternion(const btQuaternion& quat)
    {
        return {
            1 - 2 * quat.y() * quat.y() - 2 * quat.z() * quat.z(),
            2 * quat.x() * quat.y() - 2 * quat.z() * quat.w(),
            2 * quat.x() * quat.z() + 2 * quat.y() * quat.w(),
            0,

            2 * quat.x() * quat.y() + 2 * quat.z() * quat.w(),
            1 - 2 * quat.x() * quat.x() - 2 * quat.z() * quat.z(),
            2 * quat.y() * quat.z() - 2 * quat.x() * quat.w(),
            0,

            2 * quat.x() * quat.z() - 2 * quat.y() * quat.w(),
            2 * quat.y() * quat.z() + 2 * quat.x() * quat.w(),
            1 - 2 * quat.x() * quat.x() - 2 * quat.y() * quat.y(),
            0,

            0, 0, 0, 1
        };
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
    auto entityTerrain = registry_.create();
    terrainChunks_.emplace_back(entityTerrain);


    auto& entity1Transform = registry_.emplace<ray_sim::Transform>(entity1);
    entity1Transform.transform.setOrigin({0, 10, 0});
    entity1Transform.transform.setRotation(fromEuler(0, 0,0));
    const auto& physicsBody = registry_.emplace<PhysicsBody>(entity1, physicsWorld_, 1.0f, entity1Transform.transform, btBoxShape(btVector3(1, 1, 1)));
    auto& simModel = registry_.emplace<SimModel>(entity1);
    simModel.loadModelFromMesh(physicsBody.getCollisionMesh());
    simModel.setColor(WHITE);

    auto& entity3Transform = registry_.emplace<ray_sim::Transform>(entityTerrain);
    entity3Transform.transform.setOrigin({0, 0, 0});
    entity3Transform.transform.setRotation(fromEuler(entity3Transform.transform.getOrigin().getX(), 0,entity3Transform.transform.getOrigin().getY()));
    auto terrainChunk =registry_.emplace<terrain_generator::TerrainChunk>(entityTerrain, 0.0 ,0.0);
    registry_.emplace<PhysicsBody>(entityTerrain, physicsWorld_, 0.0f,entity3Transform.transform, terrainChunk.getTerrainShape());

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

    const auto terrainChunks = registry_.view<ray_sim::Transform, ray_sim::PhysicsBody, terrain_generator::TerrainChunk>();
    for(const auto& entity : terrainChunks)
    {
        auto& transform = terrainChunks.get<ray_sim::Transform>(entity).transform;
        auto& body = terrainChunks.get<ray_sim::PhysicsBody>(entity);
        auto& chunk = terrainChunks.get<terrain_generator::TerrainChunk>(entity);

        physicsWorld_.getRigidBody(body.getRigidBodyId())->getMotionState()->getWorldTransform(transform);

        // Create a Model from the Mesh
        Model& model = chunk.getModel();

        model.transform = MatrixMultiply(getRaylibRotationMatrixFromQuaternion(transform.getRotation()), MatrixTranslate(transform.getOrigin().getX() - (CHUNK_SIZE * TILE_SIZE/2.0f), 0.0f, transform.getOrigin().getZ() - (CHUNK_SIZE * TILE_SIZE/2.0f)));
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
        model.materials[0].shader = shader;
        DrawModel(model, Vector3Zero(), 1.0f, WHITE);
    }


    const auto renderEntitiesView = registry_.view<ray_sim::Transform, ray_sim::PhysicsBody, ray_sim::SimModel>();
    for(const auto& entity : renderEntitiesView)
    {
        auto& transform = renderEntitiesView.get<ray_sim::Transform>(entity).transform;
        auto& body = renderEntitiesView.get<ray_sim::PhysicsBody>(entity);
        auto& simModel = renderEntitiesView.get<ray_sim::SimModel>(entity);

        physicsWorld_.getRigidBody(body.getRigidBodyId())->getMotionState()->getWorldTransform(transform);

        // Create a Model from the Mesh
        Model& model = simModel.getModel();

        model.transform = MatrixMultiply(getRaylibRotationMatrixFromQuaternion(transform.getRotation()), MatrixTranslate(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ()));
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
