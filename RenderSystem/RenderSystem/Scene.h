#pragma once

#include <memory>
#include <string>

#include "MeshCore/Object3D.h"

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
		const Camera& getCamera() const;
		const glm::mat4& getViewMatrix() const;
		const Window* getParentWindow() const;
		MeshCore::RaySurfaceIntersection getClosestIntersection(const glm::vec3& cursorPosInWorldSpace, bool intersectSurface = true);

		void render();
		void pan(const glm::vec3& startPointInWorldSpace, const glm::vec3& endPointInWorldSpace);
		void orbit(const glm::vec3& startPointInNDC, const glm::vec3& endPointInNDC);
		void zoom(float yOffset);
		void highlightFaces(const std::vector<int>& facesIndices);
		void updateRenderData();

	private:
		void init();
		void initShaderTransformationSystem();
		void adjustCamera();
		void adjustLightPos();

	private:
		Window* mParentWindow;
		Renderer mRenderer;
		Camera mCamera;
		MeshCore::Object3D mRootObject;
	};
}