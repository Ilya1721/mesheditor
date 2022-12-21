#pragma once

#include <vector>

#include "Vertex.h"

namespace MeshCore
{
	class Mesh
	{
	public:
		__declspec(dllexport) Mesh() noexcept = default;
		__declspec(dllexport) Mesh(const std::vector<Vertex>& vertices) noexcept;

		__declspec(dllexport) const std::vector<Vertex>& getVertices() noexcept;
		__declspec(dllexport) void setVertices(const std::vector<Vertex>& vertices) noexcept;
		__declspec(dllexport) void addVertex(const Vertex& vertex) noexcept;

	private:
		std::vector<Vertex> mVertices;
	};
}

