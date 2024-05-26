#pragma once

#include "Vertex.h"

namespace std
{
	template<>
	struct std::hash<MeshCore::Vertex>
	{
		std::size_t operator()(const MeshCore::Vertex& vertex) const noexcept
		{
			return std::hash<glm::vec3>{}(vertex.pos);
		}
	};

	template<>
	struct std::hash<MeshCore::HalfEdgeVerticesPair>
	{
		std::size_t operator()(const MeshCore::HalfEdgeVerticesPair& verticesPair) const noexcept
		{
			return std::hash<MeshCore::Vertex>{}(verticesPair.first) ^ (std::hash<MeshCore::Vertex>{}(verticesPair.second) << 1);
		}
	};
}
