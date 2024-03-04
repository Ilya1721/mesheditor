#include "Vertex.h"

namespace MeshCore
{
	bool Vertex::operator==(const Vertex& other) const
	{
		return mPos == other.mPos;
	}

	Vertex operator*(const glm::mat4& transform, const Vertex& vertex)
	{
		Vertex newVertex{};
		newVertex.mPos = transform * glm::vec4(vertex.mPos, 1.0f);
		newVertex.mNormal = glm::normalize(transform * glm::vec4(vertex.mNormal, 0.0f));

		return newVertex;
	}

	void Vertex::setPos(const glm::vec3& pos)
	{
		mPos = pos;
	}

	void Vertex::setNormal(const glm::vec3& normal)
	{
		mNormal = normal;
	}

	const glm::vec3& Vertex::pos() const
	{
		return mPos;
	}

	const glm::vec3& Vertex::normal() const
	{
		return mNormal;
	}

	UniqueVertex::UniqueVertex(Vertex& originalVertex, int originalVertexIndex)
		: Vertex(originalVertex)
	{
		originalVertices.emplace_back(&originalVertex, originalVertexIndex);
		adjacentFacesNormals.insert(originalVertex.normal());
	}

	void UniqueVertex::updateOriginalVertices()
	{
		for (auto& originalVertexData : originalVertices)
		{
			originalVertexData.vertex->setPos(mPos);
			originalVertexData.vertex->setNormal(mNormal);
		}
	}

	void UniqueVertex::setPos(const glm::vec3& pos)
	{
		Vertex::setPos(pos);
		updateOriginalVertices();
	}

	void UniqueVertex::setNormal(const glm::vec3& normal)
	{
		Vertex::setNormal(normal);
		updateOriginalVertices();
	}
}