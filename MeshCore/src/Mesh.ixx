export module Mesh;

import <vector>;

import Vertex;
import RenderData;

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
		RenderData getRenderData() const noexcept;

	private:
		std::vector<Vertex> mVertices;
	};
}

