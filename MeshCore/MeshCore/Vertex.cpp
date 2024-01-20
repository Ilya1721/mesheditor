#include "Vertex.h"

namespace MeshCore
{
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
}