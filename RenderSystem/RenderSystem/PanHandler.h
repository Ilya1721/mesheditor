#pragma once

#include <glm/glm.hpp>

#include "Viewport.h"

namespace RenderSystem
{
	class PanHandler
	{
	public:
		PanHandler(Viewport* viewport) noexcept;

		void pan(const glm::vec2& startMousePos, const glm::vec2& endMousePos) noexcept;

	private:
		Viewport* mViewport;
	};
}

