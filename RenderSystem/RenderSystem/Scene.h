#pragma once

#include <memory>
#include <string>

#include "MeshCore/Object3D.h"
#include "MeshCore/AABBox.h"

#include "Camera.h"
#include "Renderer.h"

namespace RenderSystem
{
	class Window;

	class Scene final
	{
	public:
		Scene(const std::string& meshFilePath, Window* parentWindow);
		Scene(const Scene& scene) = delete;
		Scene(Scene&& scene) = delete;
		Scene& operator=(const Scene& scene) = delete;
		Scene& operator=(Scene&& scene) = delete;

		void setProjectionMatrix(const Geometry::Matrix4D& projectionMatrix);

		const Geometry::Matrix4D& getModelMatrix() const;
		const Geometry::Matrix4D& getViewMatrix() const;

		void render();
		void adjust(float fov);
		void pan(const Geometry::Vector3D& firstPoint, const Geometry::Vector3D& secondPoint);
		void zoomToPoint(const Geometry::Vector3D& unProjectedMousePos, int scrollSign);

	private:
		void init();
		void initRenderBuffer();
		void initLighting();
		void adjustCamera(const MeshCore::AABBox& bbox, float fov);
		void adjustLightPos(const MeshCore::AABBox& bbox);
		const Geometry::Matrix4D& getProjectionMatrix() const;

	private:
		Window* mParentWindow;
		Renderer mRenderer;
		Camera mCamera;
		MeshCore::Object3D mRootObject;
	};
}