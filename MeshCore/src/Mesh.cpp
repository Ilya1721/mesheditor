module Mesh;

import MeshCoreConsts;

namespace MeshCore
{
	Mesh::Mesh(const std::vector<Vertex>& vertices) noexcept
		: mVertices(vertices)
	{}

	const std::vector<Vertex>& Mesh::getVertices() const noexcept
	{
		return mVertices;
	}

	void Mesh::setVertices(const std::vector<Vertex>& vertices) noexcept
	{
		mVertices = vertices;
	}

	void Mesh::addVertex(const Vertex& vertex) noexcept
	{
		mVertices.push_back(vertex);
	}

	RenderData Mesh::getRenderData() const noexcept
	{
		static_assert(COORDINATES_PER_VERTEX == COORDINATES_PER_NORMAL && COORDINATES_PER_VERTEX == COLOR_COMPONENTS_COUNT);

		RenderData renderData;
		renderData.reserveMemory(mVertices.size() * COORDINATES_PER_VERTEX);

		for (const auto& vertex : mVertices)
		{
			for (int coordIdx = 0; coordIdx < COORDINATES_PER_VERTEX; ++coordIdx)
			{
				renderData.append(vertex.pos[coordIdx], vertex.normal[coordIdx], vertex.color[coordIdx]);
			}
		}

		return renderData;
	}
}
