#include "pch.h"

#include "PanHandler.h"

#ifdef __gl_h_
#undef __gl_h_
#endif

#include "glad.h"

#include "TestRenderer.h"

namespace RenderSystem
{
	PanHandler::PanHandler(Viewport* viewport) noexcept :
		mViewport(viewport)
	{}

	void PanHandler::pan(const glm::vec2& startMousePos, const glm::vec2& endMousePos) noexcept
	{
		auto viewportHeight = mViewport->getHeight();
		auto viewportWidth = mViewport->getWidth();

		glm::vec3 startScreenPos(startMousePos.x, viewportHeight - startMousePos.y, 0.0);
		glm::vec3 endScreenPos(endMousePos.x, viewportHeight - endMousePos.y, 0.0);

		auto unProjectedStartMousePos = mViewport->screenToWorld(startScreenPos);
		auto unProjectedEndMousePos = mViewport->screenToWorld(endScreenPos);
		auto movement = unProjectedStartMousePos - unProjectedEndMousePos;

		auto& camera = mViewport->getCamera();
		camera.translate(movement);
		camera.calcViewMatrix();
	}
}
