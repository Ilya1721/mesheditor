#pragma once

#include <string>
#include <memory>

#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include "GeometryCore/Typedefs.h"

namespace RenderSystem
{
	using namespace GeometryCore;

	class Scene;
	class Viewport;
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
		static Window* createInstance(int width, int height, const std::string& meshFilePath);

		void render();

		const std::unique_ptr<Viewport>& getViewport() const;
		Point2D getCursorPos() const;
		Point3D unProject(const Point2D& cursorPos) const;
		Point3D unProjectToCameraTargetPlane(const Point2D& cursorPos) const;
		Point3D screenCoordinatesToNDC(const Point2D& cursorPos) const;
		Point3D pointOnScreenToPointInWorldSpace(const Point2D& pointOnScreen, float depth) const;
		void enableSceneMovement(bool isEnabled);

	private:
		Window(int width, int height, const std::string& meshFilePath);

		void initGLFW();
		void initScene(const std::string& meshFilePath);
		void resizeViewport(int width, int height);

		void setCallbacks();
		static void onMouseMove(GLFWwindow* window, double cursorX, double cursorY);
		static void onMouseButton(GLFWwindow* window, int button, int action, int mods);
		static void onMouseScroll(GLFWwindow* window, double xOffset, double yOffset);
		static void onFramebufferSizeChanged(GLFWwindow* window, int width, int height);
		static void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);

	private:
		int mWidth;
		int mHeight;
		bool mSceneMovementEnabled = true;

		MouseButtonPressed mMouseButtonPressed;
		Point2D mSavedCursorPosition;

		GLFWwindow* mWindow;
		std::unique_ptr<Scene> mScene;
		std::unique_ptr<Viewport> mViewport;
		std::unique_ptr<OperationsDispatcher> mOperationsDispatcher;

		static std::unique_ptr<Window> sInstance;
	};
}

