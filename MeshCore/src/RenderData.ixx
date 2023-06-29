export module RenderData;

import <vector>;

export namespace MeshCore
{
	struct RenderData
	{
		std::vector<float> positions;
		std::vector<float> normals;

		void append(const RenderData& other);
		void append(float posCoord, float normalCoord);

		void reserveMemory(size_t elementsCount);

		std::vector<float> getCompactData() const;
	};
}
