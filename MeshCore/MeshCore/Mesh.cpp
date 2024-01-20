#include "Mesh.h"

#include "Constants.h"

namespace MeshCore
{
	Mesh::Mesh(const std::vector<Vertex>& vertices)
		: mVertices(vertices)
	{
		for (size_t vertexIdx = 2; vertexIdx < mVertices.size(); vertexIdx += 3)
		{
			createFace(vertexIdx);
		}

		setupTwinsForHalfEdges();
	}

	const std::vector<Vertex>& Mesh::getVertices() const
	{
		return mVertices;
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
		auto& vertex = mVertices[vertexIdx];
		auto vertexMapIt = mVerticesMap.find(vertex);
		if (vertexMapIt != mVerticesMap.end())
		{
			return vertexMapIt->second;
		}

		mVerticesMap.insert({ vertex, &vertex });

		return &vertex;
	}

	void Mesh::createFace(size_t lastVertexIdx)
	{
		createHalfEdgesForFace(lastVertexIdx);

		const auto& firstHalfEdgeOfFace = mHalfEdges[mHalfEdges.size() - 3];
		auto face = std::make_unique<Face>();
		face->halfEdge = firstHalfEdgeOfFace.get();
		mFaces.push_back(std::move(face));
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

	RenderData Mesh::getRenderData() const
	{
		RenderData renderData;
		renderData.reserveMemory(mVertices.size() * COORDINATES_PER_VERTEX);

		for (const auto& vertex : mVertices)
		{
			for (int coordIdx = 0; coordIdx < COORDINATES_PER_VERTEX; ++coordIdx)
			{
				renderData.append(vertex.pos[coordIdx], vertex.normal[coordIdx]);
			}
		}

		return renderData;
	}
}
