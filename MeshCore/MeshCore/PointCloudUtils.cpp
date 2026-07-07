#include "PointCloudUtils.h"

#include <eigen/Dense>

#include "AABBox.h"
#include "KDTree.h"
#include "VoxelGrid.h"

namespace
{
  using namespace MeshCore;

  constexpr size_t K = 5;

  glm::mat3 outerProduct(const glm::vec3& v)
  {
    return glm::mat3(
      v.x * v.x, v.x * v.y, v.x * v.z, v.y * v.x, v.y * v.y, v.y * v.z, v.z * v.x,
      v.z * v.y, v.z * v.z
    );
  }

  glm::vec3 getCentroid(const std::vector<Vertex>& nearestVertices, const Vertex& vertex)
  {
    glm::vec3 centroid(0.0f);
    for (const auto& nearestVertex : nearestVertices)
    {
      centroid += nearestVertex.pos;
    }
    centroid /= nearestVertices.size();

    return centroid;
  }

  glm::mat3 getCovarianceMatrix(
    const std::vector<Vertex>& nearestVertices, const glm::vec3& centroid
  )
  {
    glm::mat3 covariance(0.0f);
    for (const auto& nearestVertex : nearestVertices)
    {
      auto diff = nearestVertex.pos - centroid;
      covariance += outerProduct(diff);
    }

    return covariance;
  }

  glm::vec3 getSmallestEigenVector(const glm::mat3& covariance)
  {
    Eigen::Matrix3f eigenCovariance;
    for (int rowIdx = 0; rowIdx < 3; ++rowIdx)
    {
      for (int colIdx = 0; colIdx < 3; ++colIdx)
      {
        eigenCovariance(rowIdx, colIdx) = covariance[colIdx][rowIdx];
      }
    }

    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> solver(eigenCovariance);
    Eigen::Vector3f smallestEigenVector = solver.eigenvectors().col(0);
    glm::vec3 smallestEigenVectorGLM(
      smallestEigenVector.x(), smallestEigenVector.y(), smallestEigenVector.z()
    );

    return glm::normalize(smallestEigenVectorGLM);
  }

  glm::vec3 getOrientedNormal(
    const glm::vec3& pos, const glm::vec3& normal, const glm::vec3& bboxCenter
  )
  {
    return glm::dot(normal, pos - bboxCenter) < 0.0f ? -normal : normal;
  }

  std::vector<Vertex> setupNormalsForVertices(
    const std::vector<Vertex>& vertices, const glm::vec3& bboxCenter
  )
  {
    auto k = std::min(K, vertices.size() - 1);
    std::vector<Vertex> verticesWithNormals = vertices;
    KDTree pointKDTree(verticesWithNormals);
    for (auto& vertex : verticesWithNormals)
    {
      auto nearestVertices = pointKDTree.getNearestVertices(vertex, k);
      auto centroid = getCentroid(nearestVertices, vertex);
      auto covariance = getCovarianceMatrix(nearestVertices, centroid);
      auto normal = getSmallestEigenVector(covariance);
      vertex.normal = getOrientedNormal(vertex.pos, normal, bboxCenter);
    }
    return verticesWithNormals;
  }
}

namespace MeshCore
{
  std::vector<Vertex> getReconstructedVertices(
    const std::vector<Vertex>& pointCloud, const glm::vec3& bboxCenter
  )
  {
    auto vertices = setupNormalsForVertices(pointCloud, bboxCenter);
    VoxelGrid voxelGrid(vertices);
    return voxelGrid.getReconstructedVertices();
  }
}
