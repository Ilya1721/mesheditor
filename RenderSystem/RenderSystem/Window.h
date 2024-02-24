#pragma once

#include <glfw/glfw3.h>

#include "MeshCore/Mesh.h"

#include "Scene.h"
#include "Viewport.h"
#include "OperationsDispatcher.h"

namespace RenderSystem
{
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
		glm::vec2 getCursorPos() const;
		glm::vec3 unProject(const glm::vec2& cursorPos) const;
		glm::vec3 screenCoordinatesToNDC(const glm::vec2& cursorPos) const;

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

		MouseButtonPressed mMouseButtonPressed;
		glm::vec2 mSavedCursorPosition;

		GLFWwindow* mWindow;
		std::unique_ptr<Scene> mScene;
		std::unique_ptr<Viewport> mViewport;
		std::unique_ptr<OperationsDispatcher> mOperationsDispatcher;

		static std::unique_ptr<Window> sInstance;
	};
}

