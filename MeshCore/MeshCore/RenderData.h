#pragma once

#include <vector>

namespace MeshCore
{
	struct RenderData
	{
		std::vector<float> positions;
		std::vector<float> normals;
		std::vector<float> colors;
	};
}
