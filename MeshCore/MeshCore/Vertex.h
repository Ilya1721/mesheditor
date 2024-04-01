#pragma once

#include <unordered_set>

#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>

#include "GeometryCore/Typedefs.h"

namespace MeshCore
{
	using namespace GeometryCore;

	struct HalfEdge;

	struct Vertex
	{
		Vertex() = default;
		Vertex(const Point3D& pos, const Vector3D& normal);

		HalfEdge* halfEdge = nullptr;

		void setPos(const Point3D& pos);
		void setNormal(const Vector3D& normal);

		const Point3D& pos() const;
		const Vector3D& normal() const;

		bool operator==(const Vertex& other) const;
		friend Vertex operator*(const glm::mat4& transform, const Vertex& vertex);

	protected:
		Point3D mPos{};
		Vector3D mNormal{};
	};

	struct OriginalVertexData
	{
		Vertex* vertex;
		int index;
	};

	struct UniqueVertex : Vertex
	{
		UniqueVertex(Vertex& originalVertex, int originalVertexIndex);

		void setPos(const Point3D& pos);
		void setNormal(const Vector3D& normal);

		std::vector<OriginalVertexData> originalVertices;
		std::unordered_set<Vector3D> adjacentFacesNormals;

	private:
		void updateOriginalVertices();
	};

	using HalfEdgeVerticesPair = std::pair<Vertex, Vertex>;
}
