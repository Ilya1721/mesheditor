#pragma once

#include <glm/glm.hpp>

namespace MeshCore
{
	class Constants final
	{
	public:
		Constants() = delete;

		static const int VERTICES_PER_FACE = 3;
		static const int COORDINATES_PER_VERTEX = 3;
		static const int COORDINATES_PER_NORMAL = 3;
		static const int COLOR_COMPONENTS_COUNT = 3;
	};
}
