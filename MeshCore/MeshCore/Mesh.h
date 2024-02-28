#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "GeometryCore/Ray.h"

#include "Vertex.h"
#include "HalfEdge.h"
#include "Face.h"
#include "Surface.h"
#include "RenderData.h"
#include "VerticesHash.h"
#include "Intersection.h"
#include "Surface.h"

namespace MeshCore
{
	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(const std::vector<Vertex>& vertices);
		Mesh(const Mesh& other) = delete;
		Mesh(Mesh&& other) = default;
		Mesh& operator=(const Mesh& other) = delete;
		Mesh& operator=(Mesh&& other) = default;
		bool operator==(const Mesh& other) const = default;

		void updateVertices(const std::unordered_set<Vertex*>& vertices);

		const std::vector<Vertex>& getVertices() const;
		int getNumberOfFaces() const;
		const RenderData& getRenderData() const;
		RaySurfaceIntersection getClosestIntersection(const GeometryCore::Ray& ray, bool intersectSurface, int passedFacesCount = 0) const;

	private:
		void init();
		void prepareRenderData();
		void createFace(size_t lastVertexIdx);
		void createHalfEdgesForFace(size_t lastVertexIdx);
		void createHalfEdgeVerticesMap(const std::unique_ptr<HalfEdge>& firstHalfEdge,
									   const std::unique_ptr<HalfEdge>& secondHalfEdge,
									   const std::unique_ptr<HalfEdge>& thirdHalfEdge);
		void createHalfEdgeLoop(const std::unique_ptr<HalfEdge>& firstHalfEdge,
							    const std::unique_ptr<HalfEdge>& secondHalfEdge,
							    const std::unique_ptr<HalfEdge>& thirdHalfEdge);
		void connectVertexAndHalfEdge(Vertex* vertex, const std::unique_ptr<HalfEdge>& halfEdge);
		void setupTwinsForHalfEdges();
		Vertex* getUniqueVertex(size_t vertexIdx);
		void averageFaceNormals();
		void connectHalfEdgesToFace();
		void updateVertices();
		std::vector<int> getIntersectedSurfaceIndices(const Surface& surface) const;
		void addFace();

	private:
		std::vector<Vertex> mVertices;
		std::vector<std::unique_ptr<HalfEdge>> mHalfEdges;
		std::vector<std::unique_ptr<Face>> mFaces;
		std::unordered_map<HalfEdgeVerticesPair, HalfEdge*> mHalfEdgeVerticesMap;
		std::unordered_map<Vertex, VertexWithExtraData> mUniqueVerticesMap;
		std::unordered_map<Face*, int> mFaceIndexMap;
		std::unordered_map<Vertex*, int> mVertexIndexMap;
		RenderData mRenderData;
	};
}
