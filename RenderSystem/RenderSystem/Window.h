#pragma once

#include <string>
#include <memory>
#include <functional>

#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include "GeometryCore/Typedefs.h"
#include "GeometryCore/Ray.h"
#include "Utility/CallbackMechanism.h"

#include "Constants.h"

namespace RenderSystem
{
	using namespace GeometryCore;
	using namespace Utility;

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
		static Window& getInstance();

		Point2D getCursorPos() const;
		Point3D unProject(const Point2D& cursorPos, float depth = DEFAULT_Z_VALUE) const;
		Point3D screenCoordinatesToNDC(const Point2D& cursorPos, float depth = DEFAULT_Z_VALUE) const;
		Ray castCursorRay() const;
		bool isMouseButtonPressed(int button) const;
		bool isKeyPressed(int key) const;
		int getWidth() const;
		int getHeight() const;

		void init(int width, int height, const std::string& meshFilePath);
		void render();
		void onMouseMove(double cursorX, double cursorY);
		void onMouseButton(int button, int action, int mods);
		void onMouseScroll(double xOffset, double yOffset);
		void onFramebufferSizeChanged(int width, int height);
		void onKey(int key, int scancode, int action, int mods);
		void addWindowInitializedCallback(const std::function<void()>& callback);

	private:
		~Window();

		void initGLFW();
		void initSceneAndViewport(const std::string& meshFilePath);
		void resizeViewport(int width, int height);
		void setGLFWCallbacks();

	private:
		int mWidth;
		int mHeight;
		Point2D mSavedCursorPosition;

		GLFWwindow* mWindow;
		std::unique_ptr<OperationsDispatcher> mOperationsDispatcher;
		CallbackMechanism mInitializationCallbackMechanism;

		static Window sInstance;
	};
}

