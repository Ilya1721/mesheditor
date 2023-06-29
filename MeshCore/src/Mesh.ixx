export module Mesh;

import <vector>;

import Vertex;
import RenderData;

export namespace MeshCore
{
	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(const std::vector<Vertex>& vertices);

		const std::vector<Vertex>& getVertices() const;
		void addVertex(const Vertex& vertex);
		RenderData getRenderData() const;

	private:
		std::vector<Vertex> mVertices;
	};
}

