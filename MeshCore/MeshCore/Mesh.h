#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <array>
#include <optional>

#include "VerticesHash.h"
#include "Intersection.h"

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
		Mesh() = default;
		Mesh(const std::vector<Vertex>& vertices);
		Mesh(Mesh&& other) = delete;
		~Mesh();
		bool operator==(const Mesh& other) const = default;

		std::unique_ptr<Mesh> clone() const;

		const std::vector<Vertex>& getVertices() const;
		const std::vector<std::unique_ptr<Face>>& getFaces() const;
		MeshIntersectionData findIntersection(const Ray& ray, bool intersectSurface, int passedFacesCount = 0) const;

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
		std::optional<Point3D> getIntersectionPoint(const Ray& ray) const;

	private:
		std::vector<Vertex> mVertices;
		std::vector<std::unique_ptr<HalfEdge>> mHalfEdges;
		std::vector<std::unique_ptr<Face>> mFaces;
		std::unordered_map<Vertex, UniqueVertex> mUniqueVerticesMap;
		std::unordered_map<Face*, int> mFaceIndexMap;
		mutable IntersectionTempData mIntersectionTempData;
	};
}
