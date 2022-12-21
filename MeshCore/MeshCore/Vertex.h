#pragma once

#include <glm/glm.hpp>

namespace MeshCore
{
	struct Vertex
	{
		glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
		glm::vec3 normal = { 0.0f, 0.0f, 1.0f };
		glm::vec3 color = { 1.0f, 1.0f, 1.0f };
	};
}
