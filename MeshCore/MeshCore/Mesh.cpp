#include "Mesh.h"

#include "Constants.h"
#include "Face.h"
#include "GeometryCore/Numeric.h"
#include "GeometryCore/Ray.h"
#include "HalfEdge.h"
#include "Surface.h"

using namespace GeometryCore;

namespace
{
  using namespace MeshCore;

  Vector3D calcAverage(const std::unordered_set<Vector3D>& vecSet)
  {
    Vector3D result {};
    for (const auto& vec : vecSet) { result += vec; }

    return result / static_cast<float>(vecSet.size());
  }

  void createHalfEdgeLoop(const std::array<HalfEdge*, 3>& halfEdges)
  {
    halfEdges[0]->next = halfEdges[1];
    halfEdges[0]->prev = halfEdges[2];
    halfEdges[1]->next = halfEdges[2];
    halfEdges[1]->prev = halfEdges[0];
    halfEdges[2]->next = halfEdges[0];
    halfEdges[2]->prev = halfEdges[1];
  }

  void connectVertexAndHalfEdge(
    UniqueVertex* vertex, const std::unique_ptr<HalfEdge>& halfEdge
  )
  {
    vertex->halfEdge = halfEdge.get();
    halfEdge->vertex = vertex;
  }
}  // namespace

namespace MeshCore
{
  Mesh::Mesh() : mMaterialParams(GOLD_MATERIAL), mIsManifold(true) {}

  Mesh::Mesh(
    const std::vector<Vertex>& vertices, const BlinnPhongMaterialParams& materialParams
  )
    : mVertices(vertices), mMaterialParams(materialParams), mIsManifold(true)
  {
    init();
  }

  Mesh::~Mesh() = default;

  void Mesh::init() { prepareHalfEdgeDataStructure(); }

  void Mesh::prepareHalfEdgeDataStructure()
  {
    for (size_t vertexIdx = 2; vertexIdx < mVertices.size(); vertexIdx += 3)
    {
      createFace(vertexIdx);
    }

    setupTwinsForHalfEdges();

    if (SMOOTHING_ENABLED) { averageFaceNormals(); }
  }

  std::unique_ptr<Mesh> Mesh::clone() const { return std::make_unique<Mesh>(mVertices); }

  void Mesh::setMaterialParams(const BlinnPhongMaterialParams& materialParams)
  {
    mMaterialParams = materialParams;
  }

  const std::vector<Vertex>& Mesh::getVertices() const { return mVertices; }

  const std::vector<std::unique_ptr<Face>>& Mesh::getFaces() const { return mFaces; }

  void Mesh::createHalfEdgesForFace(size_t lastVertexIdx)
  {
    auto firstUniqueVertex = getUniqueVertex(lastVertexIdx - 2);
    auto secondUniqueVertex = getUniqueVertex(lastVertexIdx - 1);
    auto thirdUniqueVertex = getUniqueVertex(lastVertexIdx);

    auto firstHalfEdge = std::make_unique<HalfEdge>();
    auto secondHalfEdge = std::make_unique<HalfEdge>();
    auto thirdHalfEdge = std::make_unique<HalfEdge>();

    connectVertexAndHalfEdge(firstUniqueVertex, firstHalfEdge);
    connectVertexAndHalfEdge(secondUniqueVertex, secondHalfEdge);
    connectVertexAndHalfEdge(thirdUniqueVertex, thirdHalfEdge);
    createHalfEdgeLoop({firstHalfEdge.get(), secondHalfEdge.get(), thirdHalfEdge.get()});

    mHalfEdges.push_back(std::move(firstHalfEdge));
    mHalfEdges.push_back(std::move(secondHalfEdge));
    mHalfEdges.push_back(std::move(thirdHalfEdge));
  }

  std::unordered_map<HalfEdgeVerticesPair, HalfEdge*> Mesh::createHalfEdgeVerticesMap(
  ) const
  {
    std::unordered_map<HalfEdgeVerticesPair, HalfEdge*> halfEdgeVerticesMap;

    for (size_t halfEdgeIdx = 0; halfEdgeIdx < mHalfEdges.size(); halfEdgeIdx += 3)
    {
      auto firstVerticesPair = std::make_pair(
        *mHalfEdges[halfEdgeIdx]->vertex, *mHalfEdges[halfEdgeIdx + 1]->vertex
      );
      auto secondVerticesPair = std::make_pair(
        *mHalfEdges[halfEdgeIdx + 1]->vertex, *mHalfEdges[halfEdgeIdx + 2]->vertex
      );
      auto thirdVerticesPair = std::make_pair(
        *mHalfEdges[halfEdgeIdx + 2]->vertex, *mHalfEdges[halfEdgeIdx]->vertex
      );

      halfEdgeVerticesMap.insert({firstVerticesPair, mHalfEdges[halfEdgeIdx].get()});
      halfEdgeVerticesMap.insert({secondVerticesPair, mHalfEdges[halfEdgeIdx + 1].get()});
      halfEdgeVerticesMap.insert({thirdVerticesPair, mHalfEdges[halfEdgeIdx + 2].get()});
    }

    return halfEdgeVerticesMap;
  }

  UniqueVertex* Mesh::getUniqueVertex(int vertexIdx)
  {
    auto& vertex = mVertices[vertexIdx];
    auto vertexMapIt = mUniqueVerticesMap.find(vertex);

    if (vertexMapIt == mUniqueVerticesMap.end())
    {
      UniqueVertex uniqueVertex(vertex, vertexIdx);
      auto& [vertexKey, insertedUniqueVertex] =
        *mUniqueVerticesMap.insert({vertex, std::move(uniqueVertex)}).first;
      return &insertedUniqueVertex;
    }

    auto& [vertexKey, uniqueVertex] = *vertexMapIt;
    uniqueVertex.adjacentFacesNormals.insert(vertex.normal);
    uniqueVertex.originalVertices.emplace_back(&vertex, vertexIdx);

    return &uniqueVertex;
  }

  void Mesh::createFace(size_t lastVertexIdx)
  {
    addFace();
    createHalfEdgesForFace(lastVertexIdx);
    connectHalfEdgesToFace();
  }

  void Mesh::addFace()
  {
    mFaces.push_back(std::make_unique<Face>(this));
    int faceIdx = mFaces.size() - 1;
    mFaceIndexMap.insert({mFaces[faceIdx].get(), faceIdx});
  }

  void Mesh::connectHalfEdgesToFace()
  {
    auto& lastFace = mFaces[mFaces.size() - 1];
    lastFace->halfEdge = mHalfEdges[mHalfEdges.size() - 3].get();

    for (size_t halfEdgeOffset = 1; halfEdgeOffset <= 3; ++halfEdgeOffset)
    {
      mHalfEdges[mHalfEdges.size() - halfEdgeOffset]->face = lastFace.get();
    }
  }

  void Mesh::setupTwinsForHalfEdges()
  {
    auto halfEdgeVerticesMap = createHalfEdgeVerticesMap();

    for (auto& halfEdge : mHalfEdges)
    {
      auto twinIt = halfEdgeVerticesMap.find(
        std::make_pair(*halfEdge->next->vertex, *halfEdge->vertex)
      );
      if (twinIt == halfEdgeVerticesMap.end())
      {
        mIsManifold = false;
        return;
      }

      auto twinHalfEdge = twinIt->second;
      halfEdge->twin = twinHalfEdge;
      twinHalfEdge->twin = halfEdge.get();
    }
  }

  void Mesh::averageFaceNormals()
  {
    for (auto& [origVertex, uniqueVertex] : mUniqueVerticesMap)
    {
      uniqueVertex.updateNormal(
        glm::normalize(calcAverage(uniqueVertex.adjacentFacesNormals))
      );
    }
  }

  MeshIntersection Mesh::getRayIntersection(
    const Ray& ray, IntersectionMode intersectionMode, int facesIndexOffset
  ) const
  {
    const auto& intersectionTempData = getIntersectionTempData(ray);

    if (!intersectionTempData.intersectionPoint) { return {}; }

    auto faceIdx = intersectionTempData.intersectedFaceIdx;
    MeshIntersection meshIntersection;

    if (intersectionMode == IntersectionMode::SURFACE)
    {
      Surface surface(mFaces[faceIdx].get());
      meshIntersection.intersectedFacesIndices = getIntersectedSurfaceIndices(surface);
      meshIntersection.intersectedSurface = surface;
    }
    else
    {
      meshIntersection.intersectedFacesIndices =
        std::vector<int> {faceIdx + facesIndexOffset};
    }

    meshIntersection.intersectionPoint = intersectionTempData.intersectionPoint.value();

    return meshIntersection;
  }

  std::vector<int> Mesh::getIntersectedSurfaceIndices(const Surface& surface) const
  {
    std::vector<int> indices;
    for (const auto& face : surface.faces)
    {
      const auto& faceIndexMapIt = mFaceIndexMap.find(face);
      if (faceIndexMapIt != mFaceIndexMap.end())
      {
        const auto& [facePtr, faceIdx] = *faceIndexMapIt;
        indices.push_back(faceIdx);
      }
    }

    return indices;
  }

  MeshIntersectionTempData Mesh::getIntersectionTempData(const Ray& ray) const
  {
    MeshIntersectionTempData finalTempData;

    for (int faceIdx = 0; faceIdx < mFaces.size(); ++faceIdx)
    {
      auto intersectionPoint = mFaces[faceIdx]->getIntersectionPoint(ray);
      if (!intersectionPoint) { continue; }

      if (!finalTempData.intersectionPoint ||
          isCloser(
            intersectionPoint.value(), finalTempData.intersectionPoint.value(), ray.origin
          ))
      {
        finalTempData.intersectionPoint = intersectionPoint;
        finalTempData.intersectedFaceIdx = faceIdx;
      }
    }

    return finalTempData;
  }

  const BlinnPhongMaterialParams& Mesh::getMaterialParams() const
  {
    return mMaterialParams;
  }

  bool Mesh::isManifold() const
  {
    return mIsManifold;
  }
}  // namespace MeshCore
