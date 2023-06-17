export module RenderData;

import <vector>;

export namespace MeshCore
{
	struct RenderData
	{
		std::vector<double> positions;
		std::vector<double> normals;
		std::vector<double> colors;
	};
}
