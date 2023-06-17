export module PanHandler;

import Viewport;

export namespace RenderSystem
{
	class PanHandler
	{
	public:
		PanHandler(Viewport* viewport) noexcept;

		void pan(const Geometry::Vector2D& startMousePos, const Geometry::Vector2D& endMousePos) noexcept;

	private:
		Viewport* mViewport;
	};
}

