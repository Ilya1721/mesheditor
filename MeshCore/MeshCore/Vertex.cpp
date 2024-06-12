#include "Vertex.h"

namespace MeshCore
{
	Vertex::Vertex(const Point3D& otherPos, const Vector3D& otherNormal) :
		pos(otherPos), normal(otherNormal)
	{}

	bool Vertex::operator==(const Vertex& other) const
	{
		return pos == other.pos;
	}

	Vertex operator*(const glm::mat4& transform, const Vertex& vertex)
	{
		Vertex newVertex{};
		newVertex.pos = transform * glm::vec4(vertex.pos, 1.0f);
		newVertex.normal = glm::normalize(transform * glm::vec4(vertex.normal, 0.0f));

		return newVertex;
	}

	UniqueVertex::UniqueVertex(Vertex& originalVertex, int originalVertexIndex)
		: Vertex(originalVertex)
	{
		originalVertices.emplace_back(&originalVertex, originalVertexIndex);
		adjacentFacesNormals.insert(originalVertex.normal);
	}

	void UniqueVertex::updatePos(const Point3D& otherPos)
	{
		pos = otherPos;
		for (auto& originalVertexData : originalVertices)
		{
			originalVertexData.vertex->pos = pos;
		}
	}

	void UniqueVertex::updateNormal(const Vector3D& otherNormal)
	{
		normal = otherNormal;
	}
}