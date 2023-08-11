#pragma once

#include <vector>

#include "Vertex.h"
#include "RenderData.h"

namespace MeshCore
{
	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(const std::vector<Vertex>& vertices);

		bool operator==(const Mesh& other) const = default;

		const std::vector<Vertex>& getVertices() const;
		void addVertex(const Vertex& vertex);
		RenderData getRenderData() const;

	private:
		std::vector<Vertex> mVertices;
	};
}

