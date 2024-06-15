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
		Window(int width, int height, const std::string& meshFilePath);
		~Window();

		const std::unique_ptr<Viewport>& getViewport() const;
		Point2D getCursorPos() const;
		Point3D unProject(const Point2D& cursorPos) const;
		Point3D screenCoordinatesToNDC(const Point2D& cursorPos) const;

		void render();
		void enableSceneMovement(bool isEnabled);

		void onMouseMove(double cursorX, double cursorY);
		void onMouseButton(int button, int action, int mods);
		void onMouseScroll(double xOffset, double yOffset);
		void onFramebufferSizeChanged(int width, int height);
		void onKey(int key, int scancode, int action, int mods);

	private:
		void initGLFW();
		void initScene(const std::string& meshFilePath);
		void resizeViewport(int width, int height);
		void setCallbacks();

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
	};
}

