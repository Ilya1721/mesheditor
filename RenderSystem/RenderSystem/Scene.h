#pragma once

#include <memory>
#include <string>
#include <functional>

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
		Scene(Scene&& scene) = delete;

		Camera& getCamera();
		Window* getParentWindow();
		Renderer& getRenderer();
		MeshCore::Object3D& getRootObject();
		MeshCore::RaySurfaceIntersection getClosestIntersection(bool intersectSurface = true);
		bool isCameraMovementEnabled() const;
		MeshCore::Object3D* getPickedObject() const;

		void setPickedObject(MeshCore::Object3D* pickedObject);
		void adjustCameraAndLight();
		void updateRenderBuffer();
		void enableCameraMovement(bool isEnabled);
		void invokeModelTransformAction(const std::function<void()>& action);

	private:
		void init();
		void adjustCamera();
		void adjustLightPos();

	private:
		Window* mParentWindow;
		Renderer mRenderer;
		Camera mCamera;
		MeshCore::Object3D mRootObject;
		MeshCore::Object3D* mPickedObject;
		bool mCameraMovementEnabled;
	};
}