#include "VoxelGrid.h"

#include <Eigen/Core>
#include <Eigen/IterativeLinearSolvers>
#include <glm/gtx/norm.hpp>

#include "AABBox.h"
#include "MarchingCubesTables.h"

namespace
{
  using namespace MeshCore;

  using TripletType = Eigen::Triplet<float>;

  glm::vec3 averageNormal(const std::vector<Vertex>& vertices)
  {
    glm::vec3 normal(0.0f);
    for (const auto& vertex : vertices)
    {
      normal += vertex.normal;
    }
    if (glm::length(normal) > 0.0f)
    {
      auto verticesSize = static_cast<float>(vertices.size());
      normal = glm::normalize(normal / verticesSize);
    }

    return normal;
  }

  int getCubeIndex(const float values[CUBE_CORNERS], float iso)
  {
    int cubeIndex = 0;
    for (int valueIdx = 0; valueIdx < CUBE_CORNERS; ++valueIdx)
    {
      if (values[valueIdx] > iso)
      {
        cubeIndex |= (1 << valueIdx);
      }
    }

    return cubeIndex;
  }

  glm::vec3 getVertexPosFromEdgeVertices(
    const glm::vec3 edgeVertices[12], int cubeIdx, int vertexIdx
  )
  {
    auto edgeVertexIdx = MC_TRI_TABLE[cubeIdx][vertexIdx];
    return edgeVertices[edgeVertexIdx];
  }

  glm::vec3 vertexInterpolation(
    float iso,
    const glm::vec3& leftPoint,
    const glm::vec3& rightPoint,
    float leftValue,
    float rightValue
  )
  {
    if (glm::epsilonEqual(leftValue, rightValue, EPSILON))
    {
      return leftPoint;
    }

    float t = (iso - leftValue) / (rightValue - leftValue);
    return leftPoint + t * (rightPoint - leftPoint);
  }

  std::vector<Vertex> getFaceVertices(
    const glm::vec3 edgeVertices[12], int cubeIdx, int startVertexIdx
  )
  {
    auto pos0 = getVertexPosFromEdgeVertices(edgeVertices, cubeIdx, startVertexIdx);
    auto pos1 = getVertexPosFromEdgeVertices(edgeVertices, cubeIdx, startVertexIdx + 1);
    auto pos2 = getVertexPosFromEdgeVertices(edgeVertices, cubeIdx, startVertexIdx + 2);
    auto edge01 = pos1 - pos0;
    auto edge02 = pos2 - pos0;
    auto normal = glm::normalize(glm::cross(edge01, edge02));

    return {Vertex(pos0, normal), Vertex(pos1, normal), Vertex(pos2, normal)};
  }

  std::vector<Vertex> getSurfaceVertices(
    const glm::vec3 corners[CUBE_CORNERS], const float values[CUBE_CORNERS], float iso
  )
  {
    auto cubeIdx = getCubeIndex(values, iso);
    if (MC_EDGE_TABLE[cubeIdx] == 0)
    {
      return {};
    }

    glm::vec3 edgeVertices[12];
    for (int edgeIdx = 0; edgeIdx < 12; ++edgeIdx)
    {
      if (!(MC_EDGE_TABLE[cubeIdx] & (1 << edgeIdx)))
      {
        continue;
      }
      const auto [left, right] = MC_EDGE_CORNERS[edgeIdx];
      edgeVertices[edgeIdx] = vertexInterpolation(
        iso, corners[left], corners[right], values[left], values[right]
      );
    }

    std::vector<Vertex> vertices;
    for (int vertexIdx = 0; MC_TRI_TABLE[cubeIdx][vertexIdx] != -1; vertexIdx += 3)
    {
      auto faceVertices = getFaceVertices(edgeVertices, cubeIdx, vertexIdx);
      vertices.insert(vertices.end(), faceVertices.begin(), faceVertices.end());
    }

    return vertices;
  }

  float derivative(
    const Voxel* center,
    const Voxel* plus,
    const Voxel* minus,
    int component,
    float spacing
  )
  {
    auto centerValue = center->accumulatedNormal[component];
    auto plusValue = plus ? plus->accumulatedNormal[component] : 0.0f;
    auto minusValue = minus ? minus->accumulatedNormal[component] : 0.0f;
    if (plus && minus)
    {
      return (plusValue - minusValue) / (2.0f * spacing);
    }
    if (plus)
    {
      return (plusValue - centerValue) / spacing;
    }
    if (minus)
    {
      return (centerValue - minusValue) / spacing;
    }

    return 0.0f;
  }

  float getDivergence(
    const Voxel& voxel, const FaceNeighbors& neighbors, const glm::vec3& voxelSize
  )
  {
    float dx = derivative(&voxel, neighbors.xp, neighbors.xm, 0, voxelSize.x);
    float dy = derivative(&voxel, neighbors.yp, neighbors.ym, 1, voxelSize.y);
    float dz = derivative(&voxel, neighbors.zp, neighbors.zm, 2, voxelSize.z);
    return dx + dy + dz;
  }
}

namespace MeshCore
{
  VoxelGrid::VoxelGrid(const std::vector<Vertex>& vertices)
    : mMinBound(std::numeric_limits<float>::max()),
      mMaxBound(std::numeric_limits<float>::min())
  {
    setupResolution(vertices.size());
    setupBounds(vertices);
    setupVoxelSize();
    allocateVoxels();
    for (const auto& vertex : vertices)
    {
      insert(vertex);
    }
  }

  void VoxelGrid::setupBounds(const std::vector<Vertex>& vertices)
  {
    for (const auto& vertex : vertices)
    {
      mMinBound = glm::min(mMinBound, vertex.pos);
      mMaxBound = glm::max(mMaxBound, vertex.pos);
    }
    auto size = mMaxBound - mMinBound;
    auto padding = glm::max(0.1f * size, glm::vec3(0.1f));
    mMinBound -= padding;
    mMaxBound += padding;
  }

  void VoxelGrid::setupAccumulatedNormals()
  {
    for (auto& voxel : mVoxels)
    {
      voxel.accumulatedNormal = averageNormal(voxel.vertices);
    }
  }

  void VoxelGrid::setupVoxelSize()
  {
    mVoxelSize.x = (mMaxBound.x - mMinBound.x) / mResolution.x;
    mVoxelSize.y = (mMaxBound.y - mMinBound.y) / mResolution.y;
    mVoxelSize.z = (mMaxBound.z - mMinBound.z) / mResolution.z;
  }

  void VoxelGrid::setupResolution(size_t vertexCount)
  {
    mResolution = glm::vec3(2.0f * std::cbrtf(vertexCount));
  }

  void VoxelGrid::allocateVoxels()
  {
    mVoxels.resize(mResolution.x * mResolution.y * mResolution.z);
  }

  void VoxelGrid::setupScalarFields()
  {
    using SpMat = Eigen::SparseMatrix<float>;
    Eigen::VectorXf rhs(mVoxels.size());
    std::vector<TripletType> triplets;
    SpMat equationsMatrix(mVoxels.size(), mVoxels.size());
    for (size_t z = 0; z < mResolution.z; ++z)
    {
      for (size_t y = 0; y < mResolution.y; ++y)
      {
        for (size_t x = 0; x < mResolution.x; ++x)
        {
          const auto& voxel = at(x, y, z);
          auto row = index(x, y, z);
          auto neighbors = getVoxelNeighbors(x, y, z);
          rhs[row] = getDivergence(voxel, neighbors, mVoxelSize);
          auto voxelTriplets = getVoxelTriplets(x, y, z);
          triplets.insert(triplets.end(), voxelTriplets.begin(), voxelTriplets.end());
        }
      }
    }
    auto mean = rhs.sum() / rhs.size();
    for (size_t row = 0; row < rhs.size(); ++row)
    {
      rhs[row] -= mean;
    }
    equationsMatrix.setFromTriplets(triplets.begin(), triplets.end());
    Eigen::ConjugateGradient<SpMat> solver;
    solver.compute(equationsMatrix);
    Eigen::VectorXf x = solver.solve(rhs);
    for (size_t voxelIdx = 0; voxelIdx < mVoxels.size(); ++voxelIdx)
    {
      mVoxels[voxelIdx].scalarField = x[voxelIdx];
    }
  }

  std::vector<TripletType> VoxelGrid::getVoxelTriplets(int x, int y, int z)
  {
    std::vector<TripletType> triplets;
    int row = index(x, y, z);
    int diag = 0;
    auto addNeighbor = [this, &row, &diag, &triplets](int nx, int ny, int nz)
    {
      if (nx < 0 || nx >= mResolution.x)
      {
        return;
      }
      if (ny < 0 || ny >= mResolution.y)
      {
        return;
      }
      if (nz < 0 || nz >= mResolution.z)
      {
        return;
      }
      int col = index(nx, ny, nz);
      triplets.emplace_back(row, col, -1.0f);
      ++diag;
    };

    addNeighbor(x + 1, y, z);
    addNeighbor(x - 1, y, z);
    addNeighbor(x, y + 1, z);
    addNeighbor(x, y - 1, z);
    addNeighbor(x, y, z + 1);
    addNeighbor(x, y, z - 1);
    triplets.emplace_back(row, row, diag);

    return triplets;
  }

  void VoxelGrid::buildCubeFromLeaf(
    const Voxel* voxel, glm::vec3 corners[CUBE_CORNERS], float values[CUBE_CORNERS]
  ) const
  {
    corners[0] = {mMinBound.x, mMinBound.y, mMinBound.z};
    corners[1] = {mMaxBound.x, mMinBound.y, mMinBound.z};
    corners[2] = {mMaxBound.x, mMaxBound.y, mMinBound.z};
    corners[3] = {mMinBound.x, mMaxBound.y, mMinBound.z};
    corners[4] = {mMinBound.x, mMinBound.y, mMaxBound.z};
    corners[5] = {mMaxBound.x, mMinBound.y, mMaxBound.z};
    corners[6] = {mMaxBound.x, mMaxBound.y, mMaxBound.z};
    corners[7] = {mMinBound.x, mMaxBound.y, mMaxBound.z};
    for (int cornerIdx = 0; cornerIdx < CUBE_CORNERS; ++cornerIdx)
    {
      values[cornerIdx] = sampleScalarField(corners[cornerIdx]);
    }
  }

  float VoxelGrid::sampleScalarField(const glm::vec3& point) const
  {
    auto local = point - mMinBound;
    auto x = static_cast<int>(local.x / mVoxelSize.x);
    auto y = static_cast<int>(local.y / mVoxelSize.y);
    auto z = static_cast<int>(local.z / mVoxelSize.z);

    x = std::clamp(x, 0, mResolution.x - 1);
    y = std::clamp(y, 0, mResolution.y - 1);
    z = std::clamp(z, 0, mResolution.z - 1);

    return at(x, y, z).scalarField;
  }

  FaceNeighbors VoxelGrid::getVoxelNeighbors(int x, int y, int z)
  {
    FaceNeighbors neighbors;
    if (x + 1 < mResolution.x)
    {
      neighbors.xp = &at(x + 1, y, z);
    }
    if (x > 0)
    {
      neighbors.xm = &at(x - 1, y, z);
    }
    if (y + 1 < mResolution.y)
    {
      neighbors.yp = &at(x, y + 1, z);
    }
    if (y > 0)
    {
      neighbors.ym = &at(x, y - 1, z);
    }
    if (z + 1 < mResolution.z)
    {
      neighbors.zp = &at(x, y, z + 1);
    }
    if (z > 0)
    {
      neighbors.zm = &at(x, y, z - 1);
    }

    return neighbors;
  }

  std::vector<Vertex> VoxelGrid::reconstructVertices() const
  {
    std::vector<Vertex> vertices;
    for (const auto& voxel : mVoxels)
    {
      glm::vec3 corners[8];
      float values[8];
      buildCubeFromLeaf(&voxel, corners, values);
      auto surfaceVertices = getSurfaceVertices(corners, values, MC_ISO);
      vertices.insert(vertices.end(), surfaceVertices.begin(), surfaceVertices.end());
    }
    return vertices;
  }

  std::vector<Vertex> VoxelGrid::getReconstructedVertices()
  {
    setupAccumulatedNormals();
    setupScalarFields();
    return reconstructVertices();
  }

  void VoxelGrid::insert(const Vertex& vertex)
  {
    auto x = (vertex.pos.x - mMinBound.x) / mVoxelSize.x;
    auto y = (vertex.pos.y - mMinBound.y) / mVoxelSize.y;
    auto z = (vertex.pos.z - mMinBound.z) / mVoxelSize.z;
    x = std::clamp(static_cast<int>(x), 0, mResolution.x - 1);
    y = std::clamp(static_cast<int>(y), 0, mResolution.y - 1);
    z = std::clamp(static_cast<int>(z), 0, mResolution.z - 1);
    at(x, y, z).vertices.push_back(vertex);
  }

  int VoxelGrid::index(int x, int y, int z) const
  {
    return x + mResolution.x * (y + mResolution.y * z);
  }

  Voxel& VoxelGrid::at(int x, int y, int z)
  {
    auto idx = index(x, y, z);
    return mVoxels[index(x, y, z)];
  }

  const Voxel& VoxelGrid::at(int x, int y, int z) const
  {
    return mVoxels[index(x, y, z)];
  }
}
