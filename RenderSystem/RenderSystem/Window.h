#pragma once

#include <glfw/glfw3.h>

#include "MeshCore/Mesh.h"

#include "Scene.h"
#include "Viewport.h"

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
		static Window* createInstance(const std::string& title, int width, int height, const std::string& meshFilePath);
		static Window* getInstance();

		void render();

		const std::unique_ptr<Viewport>& getViewport() const;

	private:
		Window(const std::string& title, int width, int height, const std::string& meshFilePath);

		void init();

		glm::vec2 getMousePos() const;

		void setCallbacks();
		static void onMouseMove(GLFWwindow* window, double cursorX, double cursorY);
		static void onMouseButton(GLFWwindow* window, int button, int action, int mods);
		static void onMouseScroll(GLFWwindow* window, double xOffset, double yOffset);
		static void onKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods);
		static void onFramebufferSizeChanged(GLFWwindow* window, int width, int height);

	private:
		void resizeViewport(int width, int height);
		glm::vec3 unProject(const glm::vec2& mousePos) const;
		glm::vec3 mousePosToNDC(const glm::vec2& mousePos) const;

	private:
		std::string mTitle;
		std::string mMeshFilePath;
		int mWidth;
		int mHeight;

		MouseButtonPressed mMouseButtonPressed;
		glm::vec2 mSavedCursorPosition;

		GLFWwindow* mWindow;
		std::unique_ptr<Scene> mScene;
		std::unique_ptr<Viewport> mViewport;

		static std::unique_ptr<Window> sInstance;
	};
}

