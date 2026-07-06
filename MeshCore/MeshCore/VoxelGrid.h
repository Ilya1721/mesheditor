#pragma once

#include <Eigen/Sparse>
#include <glm/glm.hpp>
#include <vector>

#include "Constants.h"
#include "Vertex.h"

namespace MeshCore
{
  struct Voxel
  {
    std::vector<Vertex> vertices;
    glm::vec3 accumulatedVector = glm::vec3(0.0f);
    float scalarField = 0.0f;
  };

  struct FaceNeighbors
  {
    Voxel* xp = nullptr;
    Voxel* xm = nullptr;
    Voxel* yp = nullptr;
    Voxel* ym = nullptr;
    Voxel* zp = nullptr;
    Voxel* zm = nullptr;
  };

  class VoxelGrid
  {
   public:
    VoxelGrid(const std::vector<Vertex>& vertices);

    std::vector<Vertex> getReconstructedVertices();

   private:
    int index(int x, int y, int z) const;
    Voxel& at(int x, int y, int z);
    const Voxel& at(int x, int y, int z) const;
    std::vector<Vertex> reconstructVertices() const;
    FaceNeighbors getVoxelNeighbors(int x, int y, int z);
    float sampleScalarField(const glm::vec3& point) const;
    void buildCubeFromLeaf(
      const Voxel* voxel, glm::vec3 corners[CUBE_CORNERS], float values[CUBE_CORNERS]
    ) const;
    std::vector<Eigen::Triplet<float>> getVoxelTriplets(int x, int y, int z);

    void insert(const Vertex& vertex);
    void setupBounds(const std::vector<Vertex>& vertices);
    void setupAccumulatedVectors();
    void setupScalarFields();
    void setupVoxelSize();
    void setupResolution(size_t vertexCount);
    void allocateVoxels();

   private:
    glm::vec3 mMinBound;
    glm::vec3 mMaxBound;
    glm::ivec3 mResolution;
    glm::vec3 mVoxelSize;
    std::vector<Voxel> mVoxels;
  };
}
