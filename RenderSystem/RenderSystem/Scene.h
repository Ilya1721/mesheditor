#pragma once

#include <memory>
#include <string>

#include "MeshCore/Object3D.h"
#include "GeometryCore/Typedefs.h"

#include "Camera.h"
#include "Renderer.h"

namespace RenderSystem
{
	using namespace GeometryCore;

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
		Window* getParentWindow() const;
		MeshCore::RaySurfaceIntersection getClosestIntersection(const Point3D& cursorPosInWorldSpace, bool intersectSurface = true);

		void render();
		void pan(const Point3D& startPointInWorldSpace, const Point3D& endPointInWorldSpace);
		void orbit(const Point3D& startPointInNDC, const Point3D& endPointInNDC);
		void zoom(float yOffset);
		void highlightFaces(const std::vector<int>& facesIndices);
		void updateRenderData();

	private:
		void init();
		void initShaderTransformationSystem();
		void initRenderBuffer();
		void adjustCamera();
		void adjustLightPos();

	private:
		Window* mParentWindow;
		Renderer mRenderer;
		Camera mCamera;
		MeshCore::Object3D mRootObject;
	};
}