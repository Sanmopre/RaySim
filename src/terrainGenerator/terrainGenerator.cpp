#include "terrainGenerator.h"

//std
#include <vector>

namespace terrain_generator {
    TerrainGenerator::TerrainGenerator()
    {
        noiseGenerator_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    }

    TerrainChunk TerrainGenerator::generateChunk(const Coordinates &generate) const
    {
        TerrainChunk chunk;

        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int x = 0; x < CHUNK_SIZE; x++)
            {
                chunk.heightValue[x][y] = noiseGenerator_.GetNoise(generate.x + x, generate.y + y);
            }
        }

        return chunk;
    }

    Mesh TerrainGenerator::generateTerrainMesh(const TerrainChunk &chunk)
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
                const auto height = static_cast<f32>(chunk.heightValue[x][y] * HEIGHT_MULTIPLIER);
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

        UploadMesh(&mesh, true);

        return mesh;
    }
}
