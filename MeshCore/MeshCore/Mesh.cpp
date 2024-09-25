#include "Mesh.h"

#include <numeric>
#include <chrono>
#include <thread>

#include "GeometryCore/Ray.h"
#include "GeometryCore/Numeric.h"

#include "Face.h"
#include "HalfEdge.h"
#include "Surface.h"
#include "Constants.h"
#include "EdgeWalker.h"
#include "Object3D.h"

using namespace GeometryCore;

namespace
{
	using namespace MeshCore;

	Vector3D calcAverage(const std::unordered_set<Vector3D>& vecSet)
	{
		Vector3D result{};	
		for (const auto& vec : vecSet)
		{
			result += vec;
		}

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

	void connectVertexAndHalfEdge(UniqueVertex* vertex, const std::unique_ptr<HalfEdge>& halfEdge)
	{
		vertex->halfEdge = halfEdge.get();
		halfEdge->vertex = vertex;
	}
}

namespace MeshCore
{
	Mesh::Mesh(const std::vector<Vertex>& vertices) : 
		mVertices(vertices),
		mIntersectionTempData{-1, 0},
		mParentObject(nullptr)
	{
		init();
	}

	Mesh::~Mesh() = default;

	void Mesh::init()
	{
		prepareRenderData();
		std::thread secondaryThread(&Mesh::prepareHalfEdgeDataStructure, this);
		secondaryThread.detach();
	}

	void Mesh::prepareRenderData()
	{
		for (const auto& vertex : mVertices)
		{
			mRenderData.append(vertex);
		}
	}

	void Mesh::prepareHalfEdgeDataStructure()
	{
		for (size_t vertexIdx = 2; vertexIdx < mVertices.size(); vertexIdx += 3)
		{
			createFace(vertexIdx);
		}

		setupTwinsForHalfEdges();

		if (SMOOTHING_ENABLED)
		{
			averageFaceNormals();
		}
	}

	void Mesh::setParentObject(Object3D* parentObject)
	{
		mParentObject = parentObject;
	}

	std::unique_ptr<Mesh> Mesh::clone() const
	{
		return std::make_unique<Mesh>(mVertices);
	}

	const std::vector<Vertex>& Mesh::getVertices() const
	{
		return mVertices;
	}

	const std::vector<std::unique_ptr<Face>>& Mesh::getFaces() const
	{
		return mFaces;
	}

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
		createHalfEdgeLoop({ firstHalfEdge.get(), secondHalfEdge.get(), thirdHalfEdge.get() });

		mHalfEdges.push_back(std::move(firstHalfEdge));
		mHalfEdges.push_back(std::move(secondHalfEdge));
		mHalfEdges.push_back(std::move(thirdHalfEdge));
	}

	std::unordered_map<HalfEdgeVerticesPair, HalfEdge*> Mesh::createHalfEdgeVerticesMap() const
	{
		std::unordered_map<HalfEdgeVerticesPair, HalfEdge*> halfEdgeVerticesMap;

		for (unsigned int halfEdgeIdx = 0; halfEdgeIdx < mHalfEdges.size(); halfEdgeIdx += 3)
		{
			auto firstVerticesPair = std::make_pair(*mHalfEdges[halfEdgeIdx]->vertex, *mHalfEdges[halfEdgeIdx + 1]->vertex);
			auto secondVerticesPair = std::make_pair(*mHalfEdges[halfEdgeIdx + 1]->vertex, *mHalfEdges[halfEdgeIdx + 2]->vertex);
			auto thirdVerticesPair = std::make_pair(*mHalfEdges[halfEdgeIdx + 2]->vertex, *mHalfEdges[halfEdgeIdx]->vertex);

			halfEdgeVerticesMap.insert({ firstVerticesPair, mHalfEdges[halfEdgeIdx].get() });
			halfEdgeVerticesMap.insert({ secondVerticesPair, mHalfEdges[halfEdgeIdx + 1].get() });
			halfEdgeVerticesMap.insert({ thirdVerticesPair, mHalfEdges[halfEdgeIdx + 2].get() });
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
			auto& [vertexKey, insertedUniqueVertex] = *mUniqueVerticesMap.insert({ vertex, std::move(uniqueVertex) }).first;
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
		mFaceIndexMap.insert({ mFaces[faceIdx].get(), faceIdx });
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
			auto twinIt = halfEdgeVerticesMap.find(std::make_pair(*halfEdge->next->vertex, *halfEdge->vertex));
			if (twinIt == halfEdgeVerticesMap.end())
			{
				throw std::exception("The mesh is not manifold");
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
			uniqueVertex.updateNormal(glm::normalize(calcAverage(uniqueVertex.adjacentFacesNormals)));
		}
	}

	const RenderData& Mesh::getRenderData() const
	{
		return mRenderData;
	}

	Object3D* Mesh::getParentObject() const
	{
		return mParentObject;
	}

	std::optional<Point3D> Mesh::findIntersectionPoint(const Ray& ray) const
	{
		std::optional<Point3D> finalIntersectionPoint;

		for (int faceIdx = 0; faceIdx < mFaces.size(); ++faceIdx)
		{
			auto& face = mFaces[faceIdx];
			auto intersectionPoint = face->findIntersectionPoint(ray);
			if (!intersectionPoint)
			{
				continue;
			}

			if (!finalIntersectionPoint.has_value() || isCloser(intersectionPoint.value(), finalIntersectionPoint.value(), ray.origin))
			{
				finalIntersectionPoint = intersectionPoint;
				mIntersectionTempData.intersectedFaceIdx = faceIdx;
			}
		}

		mIntersectionTempData.passedFacesCount += mFaces.size();

		return finalIntersectionPoint;
	}

	RaySurfaceIntersection Mesh::findIntersection(const Ray& ray, bool intersectSurface, int passedFacesCount) const
	{
		RaySurfaceIntersection rayFaceIntersection;
		auto invertedRay = glm::inverse(mParentObject->getTransform()) * ray;
		auto intersectionPoint = findIntersectionPoint(invertedRay);

		if (intersectionPoint.has_value())
		{
			auto faceIdx = mIntersectionTempData.intersectedFaceIdx;
			Surface surface(mFaces[faceIdx].get(), intersectSurface);
			auto facesIndices = intersectSurface ? getIntersectedSurfaceIndices(surface) : std::vector<int>{ faceIdx + passedFacesCount };
			rayFaceIntersection.setClosest({ std::move(surface), facesIndices, intersectionPoint.value() }, invertedRay.origin);
		}

		return rayFaceIntersection;
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
}
