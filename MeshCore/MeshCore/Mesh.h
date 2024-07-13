#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <array>

#include "GeometryCore/Intersectable.h"

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
	class Object3D;

	struct IntersectionTempData
	{
		int intersectedFaceIdx;
		int passedFacesCount;
	};

	class Mesh : public Intersectable
	{
	public:
		Mesh(const std::vector<Vertex>& vertices);
		Mesh(Mesh&& other) = delete;
		~Mesh();
		bool operator==(const Mesh& other) const = default;

		void updateVertices(const std::unordered_set<UniqueVertex*>& vertices);
		void setParentObject(Object3D* parentObject);

		const std::unordered_map<Vertex, UniqueVertex>& getUniqueVertices() const;
		const std::vector<Vertex>& getVertices() const;
		int getNumberOfFaces() const;
		const RenderData& getRenderData() const;
		Object3D* getParentObject() const;
		std::optional<Point3D> findIntersection(const Ray& ray) const override;
		RaySurfaceIntersection findIntersection(const Ray& ray, bool intersectSurface, int passedFacesCount = 0);

	private:
		void init();
		void prepareRenderData();
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

	private:
		std::vector<Vertex> mVertices;
		std::vector<std::unique_ptr<HalfEdge>> mHalfEdges;
		std::vector<std::unique_ptr<Face>> mFaces;
		std::unordered_map<Vertex, UniqueVertex> mUniqueVerticesMap;
		std::unordered_map<Face*, int> mFaceIndexMap;
		RenderData mRenderData;
		Object3D* mParentObject;
		mutable IntersectionTempData mIntersectionTempData;
	};
}
