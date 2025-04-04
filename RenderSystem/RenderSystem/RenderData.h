#pragma once

#include <vector>

#include "GeometryCore/Typedefs.h"

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
}

using namespace MeshCore;
using namespace GeometryCore;

struct TextureData
{
	Point2D vertexPos;
	Point2D texturePos;
};

namespace RenderSystem
{
	class RenderData
	{
	public:
		void append(const RenderData& other);
		void append(const Vertex& vertex);
		void append(const TextureData& textureData);
		void updateVertex(const OriginalVertexData& vertexData, int startVertexOffset);

		const std::vector<float>& getCompactData() const;
		int getVertexCount() const;

		static RenderData createRenderData(const GeometryCore::Ray& ray, float length);
		static RenderData createRenderData(const GeometryCore::Line& line, bool withArrowHead);
		static RenderData createRenderData(const GeometryCore::Plane& plane, float width, float length);

	private:
		std::vector<float> mCompactData;
	};
}
