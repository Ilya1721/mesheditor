#include "RenderData.h"

namespace MeshCore
{
	void RenderData::append(const RenderData& other)
	{
		positions.insert(std::end(positions), std::cbegin(other.positions), std::cend(other.positions));
		normals.insert(std::end(normals), std::cbegin(other.normals), std::cend(other.normals));
	}

	void RenderData::append(float posCoord, float normalCoord)
	{
		positions.emplace_back(posCoord);
		normals.emplace_back(normalCoord);
	}

	void RenderData::reserveMemory(size_t elementsCount)
	{
		positions.reserve(elementsCount);
		normals.reserve(elementsCount);
	}

	std::vector<float> RenderData::getCompactData() const
	{
		std::vector<float> data;
		data.insert(std::end(data), std::cbegin(positions), std::cend(positions));
		data.insert(std::end(data), std::cbegin(normals), std::cend(normals));
		return data;
	}

}
