#pragma once

#include <glm/glm.hpp>

namespace MeshCore
{
	struct HalfEdge;

	struct Vertex
	{
		HalfEdge* halfEdge = nullptr;

		glm::vec3 pos {};
		glm::vec3 normal {};

		bool operator==(const Vertex& other) const;
		friend Vertex operator*(const glm::mat4& transform, const Vertex& vertex);
	};

	using HalfEdgeVerticesPair = std::pair<Vertex, Vertex>;
}
