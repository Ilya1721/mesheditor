#pragma once

#include <glm/glm.hpp>

namespace RenderSystem
{
	class Constants final
	{
	public:
		Constants() = delete;

		static inline const glm::vec4 BACKGROUND_COLOR = glm::vec4(0.09f, 0.42f, 0.69f, 1.0f);
	};
}
