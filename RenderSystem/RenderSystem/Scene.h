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

		void setProjectionMatrix(const glm::mat4& projectionMatrix);

		const glm::mat4& getModelMatrix() const;
		const glm::mat4& getViewMatrix() const;

		void render();
		void adjust(float fov);
		void pan(const glm::vec3& firstPoint, const glm::vec3& secondPoint);
		void orbit(const glm::vec3& firstPoint, const glm::vec3& secondPoint);
		void zoomToPoint(const glm::vec3& unProjectedMousePos, int scrollSign);

	private:
		void init();
		void initRenderBuffer();
		void initLighting();
		void adjustCamera(const MeshCore::AABBox& bbox, float fov);
		void adjustLightPos(const MeshCore::AABBox& bbox);
		const glm::mat4& getProjectionMatrix() const;
		void moveRootObjectToOrigin();

	private:
		Window* mParentWindow;
		Renderer mRenderer;
		Camera mCamera;
		MeshCore::Object3D mRootObject;
	};
}