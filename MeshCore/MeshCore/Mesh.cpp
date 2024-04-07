#include "Mesh.h"

#include <numeric>
#include <chrono>

#include "GeometryCore/Ray.h"
#include "GeometryCore/Numeric.h"

#include "Face.h"
#include "HalfEdge.h"
#include "Surface.h"
#include "Constants.h"
#include "EdgeWalker.h"
#include "Intersection.h"

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
}

namespace MeshCore
{
	Mesh::Mesh(const std::vector<Vertex>& vertices)
		: mVertices(vertices), mIntersectionTempData{-1, 0}
	{
		init();
	}

	Mesh::~Mesh() = default;

	void Mesh::init()
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

		prepareRenderData();
	}

	void Mesh::prepareRenderData()
	{
		mRenderData.reserveMemory(mVertices.size() * COORDINATES_PER_VERTEX);

		for (const auto& vertex : mVertices)
		{
			mRenderData.append(vertex);
		}
	}

	void Mesh::updateVertices(const std::unordered_set<UniqueVertex*>& vertices)
	{
		for (auto& vertex : vertices)
		{
			for (auto& originalVertexData : vertex->originalVertices)
			{
				mRenderData.updateVertex(originalVertexData);
			}
		}
	}

	const std::unordered_map<Vertex, UniqueVertex>& Mesh::getVertices() const
	{
		return mUniqueVerticesMap;
	}

	int Mesh::getNumberOfFaces() const
	{
		return mFaces.size();
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
		createHalfEdgeVerticesMap({ firstHalfEdge.get(), secondHalfEdge.get(), thirdHalfEdge.get() });

		mHalfEdges.push_back(std::move(firstHalfEdge));
		mHalfEdges.push_back(std::move(secondHalfEdge));
		mHalfEdges.push_back(std::move(thirdHalfEdge));
	}

	void Mesh::createHalfEdgeVerticesMap(const std::array<HalfEdge*, 3>& halfEdges)
	{
		auto firstVerticesPair = std::make_pair(*halfEdges[0]->vertex, *halfEdges[1]->vertex);
		auto secondVerticesPair = std::make_pair(*halfEdges[1]->vertex, *halfEdges[2]->vertex);
		auto thirdVerticesPair = std::make_pair(*halfEdges[2]->vertex, *halfEdges[0]->vertex);

		mHalfEdgeVerticesMap.insert({ firstVerticesPair, halfEdges[0] });
		mHalfEdgeVerticesMap.insert({ secondVerticesPair, halfEdges[1] });
		mHalfEdgeVerticesMap.insert({ thirdVerticesPair, halfEdges[2] });
	}

	void Mesh::createHalfEdgeLoop(const std::array<HalfEdge*, 3>& halfEdges)
	{
		halfEdges[0]->next = halfEdges[1];
		halfEdges[0]->prev = halfEdges[2];
		halfEdges[1]->next = halfEdges[2];
		halfEdges[1]->prev = halfEdges[0];
		halfEdges[2]->next = halfEdges[0];
		halfEdges[2]->prev = halfEdges[1];
	}

	void Mesh::connectVertexAndHalfEdge(UniqueVertex* vertex, const std::unique_ptr<HalfEdge>& halfEdge)
	{
		vertex->halfEdge = halfEdge.get();
		halfEdge->vertex = vertex;
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
		uniqueVertex.adjacentFacesNormals.insert(vertex.normal());
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
		for (auto& halfEdge : mHalfEdges)
		{
			auto twinIt = mHalfEdgeVerticesMap.find(std::make_pair(*halfEdge->next->vertex, *halfEdge->vertex));
			if (twinIt == mHalfEdgeVerticesMap.end())
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
			uniqueVertex.setNormal(glm::normalize(calcAverage(uniqueVertex.adjacentFacesNormals)));
		}
	}

	const RenderData& Mesh::getRenderData() const
	{
		return mRenderData;
	}

	std::optional<Point3D> Mesh::findIntersection(const Ray& ray)
	{
		std::optional<Point3D> startIntersection;

		for (int faceIdx = 0; faceIdx < mFaces.size(); ++faceIdx)
		{
			auto& face = mFaces[faceIdx];
			auto intersection = face->findIntersection(ray);
			if (!intersection)
			{
				continue;
			}

			if (!startIntersection.has_value() || isCloser(intersection.value(), startIntersection.value(), ray.origin))
			{
				startIntersection = intersection;
				mIntersectionTempData.intersectedFaceIdx = faceIdx;
			}
		}

		mIntersectionTempData.passedFacesCount += mFaces.size();

		return startIntersection;
	}

	RaySurfaceIntersection Mesh::findIntersection(const Ray& ray, bool intersectSurface, int passedFacesCount)
	{
		RaySurfaceIntersection rayFaceIntersection;
		auto intersection = findIntersection(ray);

		if (intersection.has_value())
		{
			auto faceIdx = mIntersectionTempData.intersectedFaceIdx;
			Surface surface(mFaces[faceIdx].get(), intersectSurface);
			auto facesIndices = intersectSurface ? getIntersectedSurfaceIndices(surface) : std::vector<int>{ faceIdx + passedFacesCount };
			rayFaceIntersection.setClosest({ std::move(surface), facesIndices, intersection.value() }, ray.origin);
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
