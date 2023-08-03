#include "Mesh.h"

#include "Constants.h"

namespace MeshCore
{
	Mesh::Mesh(const std::vector<Vertex>& vertices)
		: mVertices(vertices)
	{}

	const std::vector<Vertex>& Mesh::getVertices() const
	{
		return mVertices;
	}

	void Mesh::addVertex(const Vertex& vertex)
	{
		mVertices.push_back(vertex);
		if (NORMALIZE_NORMALS)
		{
			auto& lastVertexNormal = mVertices.back().normal;
			lastVertexNormal = lastVertexNormal.getNormalized();
		}
	}

	RenderData Mesh::getRenderData() const
	{
		static_assert(COORDINATES_PER_VERTEX == COORDINATES_PER_NORMAL);

		RenderData renderData;
		renderData.reserveMemory(mVertices.size() * COORDINATES_PER_VERTEX);

		for (const auto& vertex : mVertices)
		{
			for (int coordIdx = 0; coordIdx < COORDINATES_PER_VERTEX; ++coordIdx)
			{
				renderData.append(vertex.pos[coordIdx], vertex.normal[coordIdx]);
			}
		}

		return renderData;
	}
}
