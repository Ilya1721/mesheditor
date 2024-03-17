#pragma once

#include <vector>

#include "GeometryCore/Ray.h"

#include "Vertex.h"

namespace MeshCore
{
	struct Vertex;

	struct RenderData
	{
		std::vector<float> positions;
		std::vector<float> normals;

		bool operator==(const RenderData& other) const = default;

		void append(const RenderData& other);
		void append(const Vertex& vertex);
		void updateVertex(const OriginalVertexData& vertexData);
		void reserveMemory(size_t numberOfElements);
		const std::vector<float>& getCompactData() const;

		static RenderData createRenderData(const GeometryCore::Ray& ray, float length);

	private:
		void prepareCompactData();

	private:
		std::vector<float> mCompactData;
	};
}
