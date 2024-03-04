#include "RenderData.h"

#include "Vertex.h"

namespace MeshCore
{
	void RenderData::append(const RenderData& other)
	{
		positions.insert(std::end(positions), std::cbegin(other.positions), std::cend(other.positions));
		normals.insert(std::end(normals), std::cbegin(other.normals), std::cend(other.normals));
		prepareCompactData();
	}

	void RenderData::append(float posCoord, float normalCoord)
	{
		positions.emplace_back(posCoord);
		normals.emplace_back(normalCoord);
		prepareCompactData();
	}

	void RenderData::updateVertex(const OriginalVertexData& vertexData)
	{
		for (int compactDataIdx = vertexData.index * 3, coordIdx = 0; coordIdx < 3; ++compactDataIdx, ++coordIdx)
		{
			mCompactData[compactDataIdx] = vertexData.vertex->pos()[coordIdx];
			mCompactData[compactDataIdx + positions.size()] = vertexData.vertex->normal()[coordIdx];
		}
	}

	void RenderData::reserveMemory(size_t numberOfElements)
	{
		positions.reserve(numberOfElements);
		normals.reserve(numberOfElements);
	}

	void RenderData::prepareCompactData()
	{
		std::vector<float> data (std::cbegin(positions), std::cend(positions));
		data.insert(std::end(data), std::cbegin(normals), std::cend(normals));
		mCompactData = data;
	}

	const std::vector<float>& RenderData::getCompactData() const
	{
		return mCompactData;
	}
}
