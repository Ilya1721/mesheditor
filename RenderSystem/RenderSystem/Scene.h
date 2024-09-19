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
		MeshCore::RaySurfaceIntersection getClosestIntersection(bool intersectSurface = true);
		bool isCameraMovementEnabled() const;
		MeshCore::Object3D* getPickedObject() const;

		void setPickedObject(MeshCore::Object3D* pickedObject);
		void adjustCameraAndLight();
		void enableCameraMovement(bool isEnabled);

		static MeshCore::Object3D& getRootObject();

	private:
		void init(const std::string& meshFilePath);
		void adjustCamera();
		void adjustLightPos();

	private:
		Window* mParentWindow;
		Renderer mRenderer;
		Camera mCamera;
		MeshCore::Object3D* mPickedObject;
		bool mCameraMovementEnabled;
		static MeshCore::Object3D sRootObject;
	};
}