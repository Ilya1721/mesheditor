#pragma once

#include <unordered_set>

#include <glm/gtx/hash.hpp>
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

	struct VertexWithExtraData
	{
		Vertex vertex;
		std::unordered_set<glm::vec3> adjacentFacesNormals;
	};

	using HalfEdgeVerticesPair = std::pair<Vertex, Vertex>;
}
