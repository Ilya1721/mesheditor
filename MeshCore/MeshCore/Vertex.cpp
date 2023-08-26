#include "Vertex.h"

namespace MeshCore
{
	Vertex operator*(const glm::mat4& transform, const Vertex& vertex)
	{
		Vertex newVertex(vertex);
		newVertex.pos = transform * glm::vec4(newVertex.pos, 1.0f);
		newVertex.normal = glm::normalize(transform * glm::vec4(newVertex.normal, 0.0f));
		return newVertex;
	}
}