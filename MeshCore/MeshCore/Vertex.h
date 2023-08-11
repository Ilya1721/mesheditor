#pragma once

#include <glm/glm.hpp>

namespace MeshCore
{
	struct Vertex
	{
		bool operator==(const Vertex& other) const = default;

		glm::vec3 pos;
		glm::vec3 normal;
	};
}
