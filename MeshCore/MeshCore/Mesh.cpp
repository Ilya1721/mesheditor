#include "Mesh.h"

#include <numeric>

#include "Constants.h"
#include "EdgeWalker.h"

namespace
{
	using namespace MeshCore;

	glm::vec3 calcAverage(const std::unordered_set<glm::vec3>& vecSet)
	{
		glm::vec3 result{};	
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
		: mVertices(vertices)
	{
		init();
	}

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
			updateVertices();
		}

		prepareRenderData();
	}

	void Mesh::prepareRenderData()
	{
		mRenderData.reserveMemory(mVertices.size() * COORDINATES_PER_VERTEX);

		for (const auto& vertex : mVertices)
		{
			for (int coordIdx = 0; coordIdx < COORDINATES_PER_VERTEX; ++coordIdx)
			{
				mRenderData.append(vertex.pos[coordIdx], vertex.normal[coordIdx]);
			}
		}
	}

	void Mesh::updateVertices(const std::unordered_set<Vertex*>& vertices)
	{
		std::vector<std::pair<int, Vertex*>> vertexIndexArray;

		for (const auto& vertex : vertices)
		{
			auto vertexIndexIt = mVertexIndexMap.find(vertex);
			if (vertexIndexIt != mVertexIndexMap.end())
			{
				auto& [vertexPtr, vertexIdx] = *vertexIndexIt;
				mVertices[vertexIdx] = *vertexPtr;
				vertexIndexArray.emplace_back(vertexIdx, vertexPtr);
			}
		}

		mRenderData.updateVertices(vertexIndexArray);
	}

	const std::vector<Vertex>& Mesh::getVertices() const
	{
		return mVertices;
	}

	int Mesh::getNumberOfFaces() const
	{
		return mFaces.size();
	}

	void Mesh::createHalfEdgesForFace(size_t lastVertexIdx)
	{
		auto firstVertex = getUniqueVertex(lastVertexIdx - 2);
		auto secondVertex = getUniqueVertex(lastVertexIdx - 1);
		auto thirdVertex = getUniqueVertex(lastVertexIdx);

		mVertexIndexMap.insert({ firstVertex, lastVertexIdx - 2 });
		mVertexIndexMap.insert({ secondVertex, lastVertexIdx - 1 });
		mVertexIndexMap.insert({ thirdVertex, lastVertexIdx });

		auto firstHalfEdge = std::make_unique<HalfEdge>();
		auto secondHalfEdge = std::make_unique<HalfEdge>();
		auto thirdHalfEdge = std::make_unique<HalfEdge>();

		connectVertexAndHalfEdge(firstVertex, firstHalfEdge);
		connectVertexAndHalfEdge(secondVertex, secondHalfEdge);
		connectVertexAndHalfEdge(thirdVertex, thirdHalfEdge);

		createHalfEdgeLoop(firstHalfEdge, secondHalfEdge, thirdHalfEdge);
		createHalfEdgeVerticesMap(firstHalfEdge, secondHalfEdge, thirdHalfEdge);

		mHalfEdges.push_back(std::move(firstHalfEdge));
		mHalfEdges.push_back(std::move(secondHalfEdge));
		mHalfEdges.push_back(std::move(thirdHalfEdge));
	}

	void Mesh::createHalfEdgeVerticesMap(const std::unique_ptr<HalfEdge>& firstHalfEdge,
										 const std::unique_ptr<HalfEdge>& secondHalfEdge,
										 const std::unique_ptr<HalfEdge>& thirdHalfEdge)
	{
		auto firstVerticesPair = std::make_pair(*firstHalfEdge->vertex, *secondHalfEdge->vertex);
		auto secondVerticesPair = std::make_pair(*secondHalfEdge->vertex, *thirdHalfEdge->vertex);
		auto thirdVerticesPair = std::make_pair(*thirdHalfEdge->vertex, *firstHalfEdge->vertex);

		mHalfEdgeVerticesMap.insert({ firstVerticesPair, firstHalfEdge.get() });
		mHalfEdgeVerticesMap.insert({ secondVerticesPair, secondHalfEdge.get() });
		mHalfEdgeVerticesMap.insert({ thirdVerticesPair, thirdHalfEdge.get() });
	}

	void Mesh::createHalfEdgeLoop(const std::unique_ptr<HalfEdge>& firstHalfEdge,
								  const std::unique_ptr<HalfEdge>& secondHalfEdge,
								  const std::unique_ptr<HalfEdge>& thirdHalfEdge)
	{
		firstHalfEdge->next = secondHalfEdge.get();
		firstHalfEdge->prev = thirdHalfEdge.get();
		secondHalfEdge->next = thirdHalfEdge.get();
		secondHalfEdge->prev = firstHalfEdge.get();
		thirdHalfEdge->next = firstHalfEdge.get();
		thirdHalfEdge->prev = secondHalfEdge.get();
	}

	void Mesh::connectVertexAndHalfEdge(Vertex* vertex, const std::unique_ptr<HalfEdge>& halfEdge)
	{
		vertex->halfEdge = halfEdge.get();
		halfEdge->vertex = vertex;
	}

	Vertex* Mesh::getUniqueVertex(size_t vertexIdx)
	{
		const auto& vertex = mVertices[vertexIdx];
		auto vertexMapIt = mUniqueVerticesMap.find(vertex);

		if (vertexMapIt != mUniqueVerticesMap.end())
		{
			vertexMapIt->second.adjacentFacesNormals.insert(vertex.normal);
			return &vertexMapIt->second.vertex;
		}

		auto insertedVertexWithExtraData = mUniqueVerticesMap.insert({ vertex, {vertex, {vertex.normal}} });

		return &insertedVertexWithExtraData.first->second.vertex;
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
		for (auto& [origVertex, vertexWithExtraData] : mUniqueVerticesMap)
		{
			vertexWithExtraData.vertex.normal = glm::normalize(calcAverage(vertexWithExtraData.adjacentFacesNormals));
		}
	}

	void Mesh::updateVertices()
	{
		for (auto& vertex : mVertices)
		{
			auto updatedVertexIt = mUniqueVerticesMap.find(vertex);
			if (updatedVertexIt != mUniqueVerticesMap.end())
			{
				auto& [key, updatedVertexWithExtraData] = *updatedVertexIt;
				vertex = updatedVertexWithExtraData.vertex;
			}
		}
	}

	const RenderData& Mesh::getRenderData() const
	{
		return mRenderData;
	}

	RaySurfaceIntersection Mesh::getClosestIntersection(const GeometryCore::Ray& ray, bool intersectSurface, int passedFacesCount) const
	{
		RaySurfaceIntersection rayFaceIntersection;

		for (int faceIdx = 0; faceIdx < mFaces.size(); ++faceIdx)
		{
			auto& face = mFaces[faceIdx];
			auto intersectionPoint = face->getIntersectionPoint(ray);
			if (intersectionPoint.has_value())
			{
				Surface surface(face.get(), intersectSurface);
				auto facesIndices = intersectSurface ? getIntersectedSurfaceIndices(surface) : std::vector<int>{ faceIdx + passedFacesCount };
				rayFaceIntersection.setClosest({ std::move(surface), facesIndices, intersectionPoint.value()}, ray.origin);
			}
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
