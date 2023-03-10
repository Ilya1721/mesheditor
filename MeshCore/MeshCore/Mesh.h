#pragma once

#include <vector>

#include "Utility/SolutionMacros.h"

#include "Vertex.h"

namespace MeshCore
{
	class Mesh
	{
	public:
		API Mesh() noexcept = default;
		API Mesh(const std::vector<Vertex>& vertices) noexcept;

		API const std::vector<Vertex>& getVertices() const noexcept;
		API void setVertices(const std::vector<Vertex>& vertices) noexcept;
		API void addVertex(const Vertex& vertex) noexcept;

	private:
		std::vector<Vertex> mVertices;
	};
}

