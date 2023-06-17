module;
#include <glfw/glfw3.h>
export module Window;

import <string>;
import <memory>;

import Scene;
import Viewport;
import Mesh;
import IEventHandler;

export namespace RenderSystem
{
	class Window
	{
	public:
		static Window* createInstance(const std::string& title, int width, int height) noexcept;
		static Window* getInstance() noexcept;

		virtual void setScene(std::unique_ptr<Scene> scene) noexcept;
		virtual void start() noexcept;
		virtual void subscribe(IEventHandler* eventHandler) noexcept;

	protected:
		Window(const std::string& title, int width, int height) noexcept;
		Window(const Window& other) = delete;
		Window(Window&& other) = delete;
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) = delete;

		Geometry::Vector2D getMousePos() const noexcept;

		void setCallbacks() noexcept;
		static void onMouseMove(GLFWwindow* window, double xPos, double yPos) noexcept;
		static void onMouseButton(GLFWwindow* window, int button, int action, int mods) noexcept;
		static void onMouseScroll(GLFWwindow* window, double xOffset, double yOffset) noexcept;
		static void onKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods) noexcept;

	protected:
		std::string mTitle;
		int mWidth;
		int mHeight;

		GLFWwindow* mWindow;
		std::unique_ptr<Scene> mScene;
		std::vector<IEventHandler*> mEventHandlers;
		static std::unique_ptr<Window> sInstance;
	};
}

