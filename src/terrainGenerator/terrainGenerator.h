#pragma once

#include <map>

#include "FastNoiseLite.h"
#include "../types.h"

#include "raylib.h"

#define CHUNK_SIZE 64u
#define TILE_SIZE 5.0f
#define HEIGHT_MULTIPLIER 10.0f

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
};

class TerrainGenerator
{
    public:
    TerrainGenerator();
    ~TerrainGenerator() = default;

    [[nodiscard]] TerrainChunk generateChunk(const Coordinates& generate) const;
    [[nodiscard]] static Mesh generateTerrainMesh(const TerrainChunk &chunk);

private:

    FastNoiseLite noiseGenerator_;
    std::map<Coordinates, TerrainChunk> chunkMap_;
};

}

