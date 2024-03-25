#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <array>

#include "VerticesHash.h"
#include "RenderData.h"

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
	struct RaySurfaceIntersection;

	class Mesh
	{
	public:
		Mesh() = default;
		~Mesh();
		Mesh(const std::vector<Vertex>& vertices);
		Mesh(const Mesh& other) = delete;
		Mesh(Mesh&& other) = default;
		Mesh& operator=(const Mesh& other) = delete;
		Mesh& operator=(Mesh&& other) = default;
		bool operator==(const Mesh& other) const = default;

		void updateVertices(const std::unordered_set<UniqueVertex*>& vertices);

		const std::unordered_map<Vertex, UniqueVertex>& getVertices() const;
		int getNumberOfFaces() const;
		const RenderData& getRenderData() const;
		RaySurfaceIntersection getClosestIntersection(const GeometryCore::Ray& ray, bool intersectSurface, int passedFacesCount = 0) const;

	private:
		void init();
		void prepareRenderData();
		void createFace(size_t lastVertexIdx);
		void createHalfEdgesForFace(size_t lastVertexIdx);
		void createHalfEdgeVerticesMap(const std::array<HalfEdge*, 3>& halfEdges);
		void createHalfEdgeLoop(const std::array<HalfEdge*, 3>& halfEdges);
		void connectVertexAndHalfEdge(UniqueVertex* vertex, const std::unique_ptr<HalfEdge>& halfEdge);
		void setupTwinsForHalfEdges();
		void averageFaceNormals();
		void connectHalfEdgesToFace();
		void addFace();

		UniqueVertex* getUniqueVertex(int vertexIdx);
		std::vector<int> getIntersectedSurfaceIndices(const Surface& surface) const;

	private:
		std::vector<Vertex> mVertices;
		std::vector<std::unique_ptr<HalfEdge>> mHalfEdges;
		std::vector<std::unique_ptr<Face>> mFaces;
		std::unordered_map<HalfEdgeVerticesPair, HalfEdge*> mHalfEdgeVerticesMap;
		std::unordered_map<Vertex, UniqueVertex> mUniqueVerticesMap;
		std::unordered_map<Face*, int> mFaceIndexMap;
		RenderData mRenderData;
	};
}
