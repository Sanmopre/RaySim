#pragma once

#include <map>

#include "FastNoiseLite.h"
#include "../types.h"

//raylib
#include "raylib.h"

//bullet
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

#define CHUNK_SIZE 40u
#define TILE_SIZE 1.0f
#define HEIGHT_MULTIPLIER 1.0f

namespace terrain_generator
{

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
    f64 heightValue[CHUNK_SIZE][CHUNK_SIZE] = {};
    [[nodiscard]] Mesh generateTerrainMesh();
    [[nodiscard]] btHeightfieldTerrainShape generateTerrainCollisionShape();
};

class TerrainGenerator
{
    public:
    TerrainGenerator();
    ~TerrainGenerator() = default;

    [[nodiscard]] TerrainChunk generateChunk(const Coordinates& generate) const;

private:

    FastNoiseLite noiseGenerator_;
    std::map<Coordinates, TerrainChunk> chunkMap_;
};

}

