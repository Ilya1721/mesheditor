#pragma once

#include <vector>

namespace MeshCore
{
	struct RenderData
	{
		std::vector<float> positions;
		std::vector<float> normals;

		bool operator==(const RenderData& other) const = default;

		void append(const RenderData& other);
		void append(float posCoord, float normalCoord);
		void reserveMemory(size_t numberOfElements);
		std::vector<float> getCompactData() const;
	};
}
