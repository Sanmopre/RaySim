#include "terrainGenerator.h"

//std
#include <iostream>
#include <memory>
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

        UploadMesh(&mesh, false);
        return mesh;
    }

     std::shared_ptr<btBvhTriangleMeshShape>  TerrainGenerator::generateTerrainCollisionShape(const TerrainChunk &chunk) {
        const auto triangleMesh = std::make_shared<btTriangleMesh>();

        for (u32 y = 0; y < CHUNK_SIZE - 1; ++y) {
            for (u32 x = 0; x < CHUNK_SIZE - 1; ++x) {
                // Get the heights of the 4 corners of the grid square
                const f64 heightTL = chunk.heightValue[x][y];
                const f64 heightTR = chunk.heightValue[x + 1][y];
                const f64 heightBL = chunk.heightValue[x][y + 1];
                const f64 heightBR = chunk.heightValue[x + 1][y + 1];

                // Define the 4 vertices of the square
                btVector3 vertexTL(x * TILE_SIZE, heightTL * HEIGHT_MULTIPLIER, y * TILE_SIZE);
                btVector3 vertexTR((x + 1) * TILE_SIZE, heightTR * HEIGHT_MULTIPLIER, y * TILE_SIZE);
                btVector3 vertexBL(x * TILE_SIZE, heightBL * HEIGHT_MULTIPLIER, (y + 1) * TILE_SIZE);
                btVector3 vertexBR((x + 1) * TILE_SIZE, heightBR * HEIGHT_MULTIPLIER, (y + 1) * TILE_SIZE);

                if ((vertexTL - vertexBL).length2() < 1e-6 ||
    (vertexBL - vertexTR).length2() < 1e-6 ||
    (vertexTR - vertexTL).length2() < 1e-6) {
                    std::cout << "Degenerate triangle detected: " << vertexTL << ", " << vertexBL << ", " << vertexTR << std::endl;
                    continue;
    }
                // Ensure vertices are valid
                if (vertexTL == vertexTR || vertexTL == vertexBL || vertexTR == vertexBR) {
                    std::cerr << "Error: Invalid or duplicate vertices in triangle mesh!" << std::endl;
                    continue;
                }

                // Add two triangles for the square
                triangleMesh->addTriangle(vertexTL, vertexBL, vertexTR);
                triangleMesh->addTriangle(vertexTR, vertexBL, vertexBR);
            }
        }

        // Validate triangle mesh
        if (triangleMesh->getNumTriangles() == 0) {
            std::cerr << "Error: Triangle mesh contains no triangles!" << std::endl;
        }

        // Construct and return the collision shape
        return std::make_shared<btBvhTriangleMeshShape>(triangleMesh.get(), true);
    }

    /*
    *     std::shared_ptr<btBvhTriangleMeshShape>  TerrainGenerator::generateTerrainCollisionShape(const TerrainChunk &chunk) {
        auto triangleMesh = std::make_shared<btTriangleMesh>();
        triangleMesh->addTriangle(btVector3(0, 0, 0), btVector3(1, 0, 0), btVector3(0, 0, 1));
        return std::make_shared<btBvhTriangleMeshShape>(triangleMesh.get(), true);
    }
     */

}
