#pragma once

#include <vector>

namespace GeometryCore
{
	struct Ray;
	struct Line;
	struct Plane;
}

namespace MeshCore
{
	struct Vertex;
	struct OriginalVertexData;

	struct RenderData
	{
		std::vector<float> positions;
		std::vector<float> normals;

		bool operator==(const RenderData& other) const = default;

		void append(const RenderData& other);
		void append(const Vertex& vertex);
		void updateVertex(const OriginalVertexData& vertexData);
		void reserveMemory(size_t verticesCount);
		void prepareCompactData();
		void clear();
		const std::vector<float>& getCompactData();

		static RenderData createRenderData(const GeometryCore::Ray& ray, float length);
		static RenderData createRenderData(const GeometryCore::Line& line, bool withArrowHead);
		static RenderData createRenderData(const GeometryCore::Plane& plane, float width, float length);

	private:
		std::vector<float> mCompactData;
	};
}
