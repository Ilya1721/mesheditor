export module Mesh;

import <vector>;

import Vertex;

export namespace MeshCore
{
	class Mesh
	{
	public:
		Mesh() noexcept = default;
		Mesh(const std::vector<Vertex>& vertices) noexcept;

		const std::vector<Vertex>& getVertices() const noexcept;
		void setVertices(const std::vector<Vertex>& vertices) noexcept;
		void addVertex(const Vertex& vertex) noexcept;

	private:
		std::vector<Vertex> mVertices;
	};
}

