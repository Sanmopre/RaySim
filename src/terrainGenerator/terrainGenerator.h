#pragma once

#include "FastNoiseLite.h"
#include "../types.h"

namespace terrain_generator
{

struct Coordinates
{
    u32 x;
    u32 y;
};

struct Chunk
{
    Coordinates coordinates;

};

class TerrainGenerator
{
    public:
    TerrainGenerator();
    ~TerrainGenerator();

};

}

