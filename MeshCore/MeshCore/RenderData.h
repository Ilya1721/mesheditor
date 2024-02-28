#pragma once

#include <vector>

namespace MeshCore
{
	struct Vertex;

	struct RenderData
	{
		std::vector<float> positions;
		std::vector<float> normals;

		bool operator==(const RenderData& other) const = default;

		void append(const RenderData& other);
		void append(float posCoord, float normalCoord);
		void updateVertices(const std::vector<std::pair<int, Vertex*>>& vertexIndexArray);
		void reserveMemory(size_t numberOfElements);
		const std::vector<float>& getCompactData() const;

	private:
		void prepareCompactData();

	private:
		std::vector<float> mCompactData;
	};
}
