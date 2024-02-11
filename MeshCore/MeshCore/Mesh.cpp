#include "Mesh.h"

#include <numeric>

#include "Constants.h"

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
		: mOriginalVertices(vertices)
	{
		init();
	}

	void Mesh::init()
	{
		for (size_t vertexIdx = 2; vertexIdx < mOriginalVertices.size(); vertexIdx += 3)
		{
			createFace(vertexIdx);
		}

		setupTwinsForHalfEdges();

		if (SMOOTHING_ENABLED)
		{
			averageFaceNormals();
			createVerticesToRender();
		}
	}

	const std::vector<Vertex>& Mesh::getVertices() const
	{
		return canRenderOriginalVertices() ? mOriginalVertices : mVerticesToRender;
	}

	int Mesh::getNumberOfFaces() const
	{
		return mFaces.size();
	}

	void Mesh::createHalfEdgesForFace(size_t lastVertexIdx)
	{
		auto thirdVertex = getUniqueVertex(lastVertexIdx);
		auto secondVertex = getUniqueVertex(lastVertexIdx - 1);
		auto firstVertex = getUniqueVertex(lastVertexIdx - 2);

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
		const auto& vertex = mOriginalVertices[vertexIdx];
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
		mFaces.push_back(std::make_unique<Face>());
		createHalfEdgesForFace(lastVertexIdx);
		connectHalfEdgesToFace();
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

	bool Mesh::canRenderOriginalVertices() const
	{
		return !SMOOTHING_ENABLED;
	}

	void Mesh::createVerticesToRender()
	{
		mVerticesToRender.clear();
		for (const auto& face : mFaces)
		{
			auto origHalfEdge = face->halfEdge;
			auto currentHalfEdge = origHalfEdge;

			do {
				mVerticesToRender.push_back(*currentHalfEdge->vertex);
				currentHalfEdge = currentHalfEdge->next;
			} while (currentHalfEdge != origHalfEdge);
		}
	}

	RenderData Mesh::getRenderData() const
	{
		auto& verticesToRender = canRenderOriginalVertices() ? mOriginalVertices : mVerticesToRender;

		RenderData renderData;
		renderData.reserveMemory(verticesToRender.size() * COORDINATES_PER_VERTEX);

		for (const auto& vertex : verticesToRender)
		{
			for (int coordIdx = 0; coordIdx < COORDINATES_PER_VERTEX; ++coordIdx)
			{
				renderData.append(vertex.pos[coordIdx], vertex.normal[coordIdx]);
			}
		}

		return renderData;
	}

	RayFaceIntersection Mesh::getClosestToCameraFaceIntersection(const GeometryCore::Ray& ray, const glm::vec3& cameraPos, int passedFacesCount) const
	{
		RayFaceIntersection rayFaceIntersection;

		for (int faceIdx = 0; faceIdx < mFaces.size(); ++faceIdx)
		{
			auto intersectionPoint = mFaces[faceIdx]->getIntersectionPoint(ray);
			if (intersectionPoint.has_value())
			{
				rayFaceIntersection.setClosest({ faceIdx + passedFacesCount, intersectionPoint.value() }, cameraPos);
			}
		}

		return rayFaceIntersection;
	}
}
