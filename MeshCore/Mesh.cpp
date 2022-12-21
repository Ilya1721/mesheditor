#include "pch.h"

#include "Mesh.h"

namespace MeshCore
{
	Mesh::Mesh(const std::vector<Vertex>& vertices) noexcept
		: 
		mVertices(vertices)
	{}

	const std::vector<Vertex>& Mesh::getVertices() noexcept
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
}
