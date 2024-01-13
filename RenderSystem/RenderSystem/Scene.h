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

	class Scene
	{
	public:
		Scene(const std::string& meshFilePath, Window* parentWindow);
		Scene(const Scene& scene) = delete;
		Scene(Scene&& scene) = delete;
		Scene& operator=(const Scene& scene) = delete;
		Scene& operator=(Scene&& scene) = delete;

		void setProjectionMatrix(const glm::mat4& projectionMatrix);
		const glm::mat4& getViewMatrix() const;

		void render();
		void pan(const glm::vec3& startPointInWorldSpace, const glm::vec3& endPointInWorldSpace);
		void orbit(const glm::vec3& startPointInNDC, const glm::vec3& endPointInNDC);
		void zoomToPoint(const glm::vec3& cursorPosInWorldSpace, float yOffset);

	private:
		void init();
		void initRenderBuffer();
		void initLighting();
		void initShaderTransformationSystem();
		void adjustCamera(const MeshCore::AABBox& bbox, float fov);
		void adjustLightPos(const MeshCore::AABBox& bbox);
		void moveRootObjectToOrigin();
		void adjustCameraAndLight();

	private:
		Window* mParentWindow;
		Renderer mRenderer;
		Camera mCamera;
		MeshCore::Object3D mRootObject;
	};
}