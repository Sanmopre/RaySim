#include "terrainGenerator.h"

//std
#include <algorithm>
#include <vector>

namespace terrain_generator
{

namespace
{
    FastNoiseLite getNoiseGenerator()
    {
        FastNoiseLite noiseGenerator;
        noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        noiseGenerator.SetFrequency(0.01);
        noiseGenerator.SetCellularJitter(0.01);
        noiseGenerator.SetFractalLacunarity(0.2f);
        return noiseGenerator;
    }

    const FastNoiseLite noiseGenerator_ = getNoiseGenerator();

    HeightMap getHeightMap(f64 xPos, f64 yPos)
    {
        HeightMap resultHeightMap;

        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int x = 0; x < CHUNK_SIZE; x++)
            {
                resultHeightMap[x][y] = noiseGenerator_.GetNoise(xPos + x, yPos + y) * HEIGHT_MULTIPLIER;
            }
        }

        return resultHeightMap;
    }
}

TerrainChunk::TerrainChunk(f64 xPos, f64 yPos)
    :
    heightValue{getHeightMap(xPos, yPos)}
    , mesh_{generateTerrainMesh()}
    , collisionShape_{generateTerrainCollisionShape()}
    , model_{LoadModelFromMesh(mesh_)}
{
}

const Mesh& TerrainChunk::getTerrainMesh() const noexcept
{
    return mesh_;
}

const btHeightfieldTerrainShape& TerrainChunk::getTerrainShape() const noexcept
{
    return collisionShape_;
}

Model & TerrainChunk::getModel() noexcept
{
    return model_;
}

Mesh TerrainChunk::generateTerrainMesh() const
{
    constexpr u32 vertexCount = CHUNK_SIZE * CHUNK_SIZE;
    constexpr u32 triangleCount = (CHUNK_SIZE - 1) * (CHUNK_SIZE - 1) * 2;

    // Allocate memory for mesh data
    std::vector<Vector3> vertices(vertexCount); // Vertex positions
    std::vector<Vector3> normals(vertexCount);  // Normals
    std::vector<Vector2> texCoords(vertexCount); // Texture coordinates
    std::vector<unsigned short> indices(triangleCount * 3); // Indices

    // Fill vertices, normals, and texture coordinates
    u32 index = 0;
    for (u32 y = 0; y < CHUNK_SIZE; y++) {
        for (u32 x = 0; x < CHUNK_SIZE; x++) {
            const auto height = static_cast<f32>(heightValue[x][y]);
            vertices[index] = { static_cast<f32>(x) * TILE_SIZE, height, static_cast<f32>(y) * TILE_SIZE};
            normals[index] = { 0.0f, 1.0f, 0.0f }; // Placeholder normals, to be calculated
            texCoords[index] = { static_cast<f32>(x) / (CHUNK_SIZE - 1), static_cast<f32>(y) / (CHUNK_SIZE - 1) };
            index++;
        }
    }

    // Fill indices for triangle generation
    index = 0;
    for (u32 y = 0; y < CHUNK_SIZE - 1; y++) {
        for (u32 x = 0; x < CHUNK_SIZE - 1; x++) {
            const u32 topLeft = y * CHUNK_SIZE + x;
            const u32 topRight = topLeft + 1;
            const u32 bottomLeft = (y + 1) * CHUNK_SIZE + x;
            const u32 bottomRight = bottomLeft + 1;

            // First triangle
            indices[index++] = topLeft;
            indices[index++] = bottomLeft;
            indices[index++] = topRight;

            // Second triangle
            indices[index++] = topRight;
            indices[index++] = bottomLeft;
            indices[index++] = bottomRight;
        }
    }

    // Create mesh
    Mesh mesh = {};
    mesh.vertexCount = vertexCount;
    mesh.triangleCount = triangleCount;

    // Upload vertex data to GPU
    mesh.vertices = reinterpret_cast<f32*>(vertices.data());
    mesh.normals = reinterpret_cast<f32*>(normals.data());
    mesh.texcoords = reinterpret_cast<f32*>(texCoords.data());
    mesh.indices = indices.data();

    UploadMesh(&mesh, false);
    return mesh;
}

btHeightfieldTerrainShape TerrainChunk::generateTerrainCollisionShape() const
{
    // Flattening the 2D heightValue array into a 1D array for btHeightfieldTerrainShape
    auto* heightData = new f32[CHUNK_SIZE * CHUNK_SIZE];
    for (int y = 0; y < CHUNK_SIZE; ++y)
    {
        for (int x = 0; x < CHUNK_SIZE; ++x)
        {
            heightData[y * CHUNK_SIZE + x] = static_cast<f32>(heightValue[x][y]);
        }
    }

    // Minimum and maximum height values in the heightfield
    const f32 minHeight = *std::min_element(heightData, heightData + CHUNK_SIZE * CHUNK_SIZE);
    const f32 maxHeight = *std::max_element(heightData, heightData + CHUNK_SIZE * CHUNK_SIZE);

    // Creating the heightfield terrain shape
    btHeightfieldTerrainShape terrainShape(
        CHUNK_SIZE,           // Number of heightfield points along width
        CHUNK_SIZE,          // Number of heightfield points along length
        heightData,      // Pointer to heightfield data
        1.0f,            // Height scale (scaling factor for height values)
        minHeight,       // Minimum height value
        maxHeight,       // Maximum height value
        1,               // Up axis (1 = Y-axis up)
        PHY_FLOAT,       // Data type of heightfield values
        false            // Flip quad edges (false = no flipping)
    );

    // Set local scaling of the shape (optional)
    btVector3 localScaling(TILE_SIZE, 1.0f, TILE_SIZE);
    terrainShape.setLocalScaling(localScaling);

    // Cleaning up height data as Bullet makes an internal copy
    delete[] heightData;

    return terrainShape;
}


}
