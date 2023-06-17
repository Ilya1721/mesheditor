module;
#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"
module PanHandler;

namespace RenderSystem
{
	PanHandler::PanHandler(Viewport* viewport) noexcept :
		mViewport(viewport)
	{}

	void PanHandler::pan(const Geometry::Vector2D& startMousePos, const Geometry::Vector2D& endMousePos) noexcept
	{
		auto viewportHeight = mViewport->getHeight();
		auto viewportWidth = mViewport->getWidth();
		auto& camera = mViewport->getCamera();
		const auto& modelViewMatrix = camera.getViewMatrix();
		const auto& projectionMatrix = mViewport->getProjectionMatrix();
		Geometry::Vector4D viewportDimensions { 0.0, 0.0, static_cast<double>(viewportWidth), static_cast<double>(viewportHeight) };

		const auto& projectedCenter = Geometry::Vector3D::project({ 0.0, 0.0, 0.0 }, modelViewMatrix, projectionMatrix, viewportDimensions);
		Geometry::Vector3D startScreenPos(startMousePos.x(), viewportHeight - startMousePos.y(), projectedCenter.z());
		Geometry::Vector3D endScreenPos(endMousePos.x(), viewportHeight - endMousePos.y(), projectedCenter.z());

		auto unProjectedStartMousePos = Geometry::Vector3D::unProject(startScreenPos, modelViewMatrix, projectionMatrix, viewportDimensions);
		auto unProjectedEndMousePos = Geometry::Vector3D::unProject(endScreenPos, modelViewMatrix, projectionMatrix, viewportDimensions);
		auto movement = unProjectedEndMousePos - unProjectedStartMousePos;

		camera.translate(movement);
		camera.calcViewMatrix();
	}
}
