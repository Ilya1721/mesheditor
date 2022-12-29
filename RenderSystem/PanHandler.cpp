#include "pch.h"

#include "PanHandler.h"

#include "TestRenderer.h"

namespace RenderSystem
{
	PanHandler::PanHandler(Viewport* viewport) noexcept :
		mViewport(viewport)
	{}

	void PanHandler::pan(const glm::vec2& startMousePos, const glm::vec2& endMousePos) noexcept
	{
		auto viewportHeight = mViewport->getHeight();
		auto cameraTarget = mViewport->getCamera().getTarget();
		auto cameraPos = mViewport->getCamera().getPosition();
		auto distanceToTarget = glm::distance(cameraPos, cameraTarget);

		glm::vec3 startScreenPos(startMousePos.x, viewportHeight - startMousePos.y, 0.0);
		glm::vec3 endScreenPos(endMousePos.x, viewportHeight - endMousePos.y, 0.0);

		auto modelViewMatrix = mViewport->getCamera().calcViewMatrix();
		auto projectionMatrix = mViewport->calcProjectionMatrix();

		auto viewportPos = mViewport->getPos();
		glm::vec4 viewport(viewportPos.x, viewportPos.y, mViewport->getWidth(), viewportHeight);

		auto unProjectedStartMousePos = glm::unProject(startScreenPos, modelViewMatrix, projectionMatrix, viewport);
		auto unProjectedEndMousePos = glm::unProject(endScreenPos, modelViewMatrix, projectionMatrix, viewport);

		TestRenderer::drawLine(unProjectedStartMousePos, unProjectedEndMousePos);

		auto movement = unProjectedStartMousePos - unProjectedEndMousePos;
		mViewport->getCamera().translate(movement);
	}
}
