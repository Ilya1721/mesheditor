#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "GeometryCore/Ray.h"

#include "Vertex.h"
#include "HalfEdge.h"
#include "Face.h"
#include "RenderData.h"
#include "VerticesHash.h"
#include "Intersection.h"

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

		const std::vector<Vertex>& getVertices() const;
		int getNumberOfFaces() const;
		RenderData getRenderData() const;
		RayFaceIntersection getClosestToCameraFaceIntersection(const GeometryCore::Ray& ray, const glm::vec3& cameraPos, int passedFacesCount = 0) const;

	private:
		void init();
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
		void createVerticesToRender();
		bool canRenderOriginalVertices() const;

	private:
		std::vector<Vertex> mOriginalVertices;
		std::vector<Vertex> mVerticesToRender;
		std::vector<std::unique_ptr<HalfEdge>> mHalfEdges;
		std::vector<std::unique_ptr<Face>> mFaces;
		std::unordered_map<HalfEdgeVerticesPair, HalfEdge*> mHalfEdgeVerticesMap;
		std::unordered_map<Vertex, VertexWithExtraData> mUniqueVerticesMap;
	};
}
