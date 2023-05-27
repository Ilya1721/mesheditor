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
		auto& camera = mViewport->getCamera();
		const auto& modelViewMatrix = camera.getViewMatrix();
		const auto& projectionMatrix = mViewport->getProjectionMatrix();
		glm::vec4 viewportDimensions { 0.0, 0.0, viewportWidth, viewportHeight };

		const auto& projectedCenter = glm::project({ 0.0, 0.0, 0.0 }, modelViewMatrix, projectionMatrix, viewportDimensions);
		glm::vec3 startScreenPos(startMousePos.x, viewportHeight - startMousePos.y, projectedCenter.z);
		glm::vec3 endScreenPos(endMousePos.x, viewportHeight - endMousePos.y, projectedCenter.z);

		auto unProjectedStartMousePos = glm::unProject(startScreenPos, modelViewMatrix, projectionMatrix, viewportDimensions);
		auto unProjectedEndMousePos = glm::unProject(endScreenPos, modelViewMatrix, projectionMatrix, viewportDimensions);
		auto movement = unProjectedEndMousePos - unProjectedStartMousePos;

		camera.translate(movement);
		camera.calcViewMatrix();
	}
}
