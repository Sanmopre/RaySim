#pragma once

#include <map>

#include "FastNoiseLite.h"
#include "../types.h"

//raylib
#include "raylib.h"

//bullet
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

//std
#include <array>

#define CHUNK_SIZE 20u
#define TILE_SIZE 1.0f
#define HEIGHT_MULTIPLIER 1.0f

namespace terrain_generator
{

using HeightMap = std::array<std::array<f64, CHUNK_SIZE>, CHUNK_SIZE>;

struct Coordinates
{
    f64 x = 0.0;
    f64 y = 0.0;

    // Operator < needed to be used as key in the chunk map
    bool operator<(const Coordinates& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        return y < other.y;
    }
};

struct TerrainChunk
{
public:
    TerrainChunk(f64 xPos, f64 yPos);
    ~TerrainChunk() = default;

public:
    [[nodiscard]] const Mesh& getTerrainMesh() const noexcept;
    [[nodiscard]] const btHeightfieldTerrainShape& getTerrainShape() const noexcept;
    [[nodiscard]] Model& getModel() noexcept;

private:

    [[nodiscard]] Mesh generateTerrainMesh() const;
    [[nodiscard]] btHeightfieldTerrainShape generateTerrainCollisionShape() const;

private:
    HeightMap heightValue= {};
    Mesh mesh_;
    btHeightfieldTerrainShape collisionShape_;
    Model model_;
};

class TerrainGenerator
{
    public:
    TerrainGenerator() = default;
    ~TerrainGenerator() = default;

private:

    std::map<Coordinates, TerrainChunk> chunkMap_;
};

}

