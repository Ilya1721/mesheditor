#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Constants.h"
#include "Intersection.h"
#include "MaterialParams.h"
#include "VerticesHash.h"

namespace GeometryCore
{
  struct Ray;
}

namespace MeshCore
{
  struct UniqueVertex;
  struct HalfEdge;
  struct Face;
  struct Surface;

  class Mesh
  {
   public:
    Mesh();
    Mesh(
      const std::vector<Vertex>& vertices,
      const MaterialParams& materialParams = GOLD_MATERIAL
    );
    Mesh(Mesh&& other) = delete;
    ~Mesh();
    bool operator==(const Mesh& other) const = default;

    std::unique_ptr<Mesh> clone() const;
    void setMaterialParams(const MaterialParams& materialParams);

    const std::vector<Vertex>& getVertices() const;
    const std::vector<std::unique_ptr<Face>>& getFaces() const;
    MeshIntersection getIntersection(
      const Ray& ray, IntersectionMode intersectionMode, int facesIndexOffset
    ) const;
    const MaterialParams& getMaterialParams() const;

   private:
    void init();
    void prepareHalfEdgeDataStructure();
    void createFace(size_t lastVertexIdx);
    void createHalfEdgesForFace(size_t lastVertexIdx);
    void setupTwinsForHalfEdges();
    void averageFaceNormals();
    void connectHalfEdgesToFace();
    void addFace();

    UniqueVertex* getUniqueVertex(int vertexIdx);
    std::unordered_map<HalfEdgeVerticesPair, HalfEdge*> createHalfEdgeVerticesMap() const;
    std::vector<int> getIntersectedSurfaceIndices(const Surface& surface) const;
    MeshIntersectionTempData getIntersectionTempData(const Ray& ray) const;

   private:
    std::vector<Vertex> mVertices;
    std::vector<std::unique_ptr<HalfEdge>> mHalfEdges;
    std::vector<std::unique_ptr<Face>> mFaces;
    std::unordered_map<Vertex, UniqueVertex> mUniqueVerticesMap;
    std::unordered_map<Face*, int> mFaceIndexMap;
    MaterialParams mMaterialParams;
  };
}  // namespace MeshCore
