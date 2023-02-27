#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glfw/glfw3.h>

#include "MeshCore/Mesh.h"
#include "IEventHandler.h"
#include "Viewport.h"
#include "Scene.h"

namespace RenderSystem
{
	class Window
	{
	public:
		API static Window* createInstance(const std::string& title, int width, int height) noexcept;

		API static Window* getInstance() noexcept;

		API virtual void setScene(std::unique_ptr<Scene> scene) noexcept;

		API virtual void start() noexcept;

		API virtual void subscribe(IEventHandler* eventHandler) noexcept;

	private:
		Window(const std::string& title, int width, int height) noexcept;
		Window(const Window& other) = delete;
		Window(Window&& other) = delete;
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) = delete;

		glm::vec2 getMousePos() const noexcept;

		void setCallbacks() noexcept;

		API static void onMouseMove(GLFWwindow* window, double xPos, double yPos) noexcept;
		API static void onMouseButton(GLFWwindow* window, int button, int action, int mods) noexcept;
		API static void onMouseScroll(GLFWwindow* window, double xOffset, double yOffset) noexcept;
		API static void onKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods) noexcept;

	private:
		std::string mTitle;

		int mWidth;
		int mHeight;

		GLFWwindow* mWindow;

		std::unique_ptr<Scene> mScene;

		std::vector<IEventHandler*> mEventHandlers;

		static std::unique_ptr<Window> sInstance;
	};
}

