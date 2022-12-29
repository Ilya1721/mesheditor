#pragma once

#include <glm/glm.hpp>

namespace RenderSystem
{
	struct MouseButtonsState
	{
		bool rightButtonPressed;
		bool leftButtonPressed;
		bool middleButtonPressed;
	};

	class IEventHandler
	{
	public:
		virtual void onMouseMove(const glm::vec2& mousePos, const MouseButtonsState& mouseButtonsState) = 0;
		virtual void onMouseDown(int button, const glm::vec2& mousePos) = 0;
		virtual void onMouseUp(int button) = 0;
		virtual void onMouseScroll(double yOffset) = 0;

		virtual void onKeyDown(int keyCode) = 0;
		virtual void onKeyUp(int keyCode) = 0;
	};
}
