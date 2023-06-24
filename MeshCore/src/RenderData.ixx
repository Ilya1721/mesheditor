export module RenderData;

import <vector>;

export namespace MeshCore
{
	struct RenderData
	{
		std::vector<float> positions;
		std::vector<float> normals;
		std::vector<float> colors;

		void append(const RenderData& other) noexcept;
		void append(float posCoord, float normalCoord, float colorPart) noexcept;

		void reserveMemory(size_t elementsCount) noexcept;

		std::vector<float> getCompactData() const noexcept;
	};
}
