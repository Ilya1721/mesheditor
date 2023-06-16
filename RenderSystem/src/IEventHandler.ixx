module;
#include "GeometryCore/Vector.h"
export module IEventHandler;

export namespace RenderSystem
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
		virtual void onMouseMove(const Geometry::Vector2D& mousePos, const MouseButtonsState& mouseButtonsState) = 0;
		virtual void onMouseDown(int button, const Geometry::Vector2D& mousePos) = 0;
		virtual void onMouseUp(int button) = 0;
		virtual void onMouseScroll(double yOffset) = 0;
		virtual void onKeyDown(int keyCode) = 0;
		virtual void onKeyUp(int keyCode) = 0;
	};
}
