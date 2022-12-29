#pragma once

#include <glm/glm.hpp>

namespace RenderSystem
{
	class TestRenderer
	{
	public:
		TestRenderer() = delete;

		static void drawLine(const glm::vec3& startPoint, const glm::vec3& endPoint) noexcept;
	};
}


