#pragma once

#include <glm/glm.hpp>

namespace MeshCore
{
	struct Vertex
	{
		bool operator==(const Vertex& other) const = default;
		friend Vertex operator*(const glm::mat4& transform, const Vertex& vertex);

		glm::vec3 pos;
		glm::vec3 normal;
	};
}
