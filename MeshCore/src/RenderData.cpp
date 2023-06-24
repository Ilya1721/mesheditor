module RenderData;

namespace MeshCore
{
	void RenderData::append(const RenderData& other) noexcept
	{
		positions.insert(std::end(positions), std::cbegin(other.positions), std::cend(other.positions));
		normals.insert(std::end(normals), std::cbegin(other.normals), std::cend(other.normals));
		colors.insert(std::end(colors), std::cbegin(other.colors), std::cend(other.colors));
	}

	void RenderData::append(float posCoord, float normalCoord, float colorPart) noexcept
	{
		positions.emplace_back(posCoord);
		normals.emplace_back(normalCoord);
		colors.emplace_back(colorPart);
	}

	void RenderData::reserveMemory(size_t elementsCount) noexcept
	{
		positions.reserve(elementsCount);
		normals.reserve(elementsCount);
		colors.reserve(elementsCount);	
	}

	std::vector<float> RenderData::getCompactData() const noexcept
	{
		std::vector<float> data;
		data.insert(std::end(data), std::cbegin(positions), std::cend(positions));
		data.insert(std::end(data), std::cbegin(normals), std::cend(normals));
		data.insert(std::end(data), std::cbegin(colors), std::cend(colors));
		return data;
	}

}
