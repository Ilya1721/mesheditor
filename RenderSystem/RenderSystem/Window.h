#pragma once

#include <string>
#include <memory>

#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include "GeometryCore/Typedefs.h"
#include "MeshCore/Intersection.h"

#include "Constants.h"
#include "Viewport.h"

namespace MeshCore
{
	class Object3D;
}

namespace RenderSystem
{
	using namespace GeometryCore;

	class Scene;
	class OperationsDispatcher;

	enum class MouseButtonPressed
	{
		LEFT,
		RIGHT,
		MIDDLE,
		NONE
	};

	class Window
	{
	public:
		Window(int width, int height, const std::string& meshFilePath);
		~Window();

		Point2D getCursorPos() const;
		Point3D unProject(const Point2D& cursorPos, float depth = DEFAULT_Z_VALUE) const;
		Point3D screenCoordinatesToNDC(const Point2D& cursorPos, float depth = DEFAULT_Z_VALUE) const;
		bool isMouseButtonPressed(int button) const;
		bool isKeyPressed(int key) const;
		PROJECTION_TYPE getProjectionType() const;
		float getFov() const;
		MeshCore::Object3D* getPickedObject() const;
		Point3D projectToCameraTargetPlane(const Point3D& cursorPosInWorldSpace) const;
		bool isCameraMovementEnabled() const;
		MeshCore::RaySurfaceIntersection getClosestIntersection(bool intersectSurface = true);

		void setPickedObject(MeshCore::Object3D* pickedObject);

		void render();
		void zoom(float step);
		void smoothOrbit(float xOffset, float yOffset);
		void pan(const Point3D& startPointInWorldSpace, const Point3D& endPointInWorldSpace, PROJECTION_TYPE projectionType);
		void enableCameraMovement(bool isEnabled);

		void onMouseMove(double cursorX, double cursorY);
		void onMouseButton(int button, int action, int mods);
		void onMouseScroll(double xOffset, double yOffset);
		void onFramebufferSizeChanged(int width, int height);
		void onKey(int key, int scancode, int action, int mods);

	private:
		void initGLFW();
		void initSceneAndViewport(const std::string& meshFilePath);
		void resizeViewport(int width, int height);
		void setCallbacks();

	private:
		int mWidth;
		int mHeight;
		Point2D mSavedCursorPosition;

		GLFWwindow* mWindow;
		std::unique_ptr<Scene> mScene;
		std::unique_ptr<Viewport> mViewport;
		std::unique_ptr<OperationsDispatcher> mOperationsDispatcher;
	};
}

