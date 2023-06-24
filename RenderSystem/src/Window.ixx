module;
#include <glfw/glfw3.h>
#include "GeometryCore/Vector.h"
export module Window;

import <string>;
import <memory>;

import Scene;
import Viewport;
import Mesh;

export namespace RenderSystem
{
	struct MouseButtonsState
	{
		bool rightButtonPressed;
		bool leftButtonPressed;
		bool middleButtonPressed;
	};

	class Window final
	{
	public:
		static Window* createInstance(const std::string& title, int width, int height, const std::string& meshFilePath) noexcept;
		static Window* getInstance() noexcept;

		void render();

		const std::unique_ptr<Viewport>& getViewport() const noexcept;

	private:
		Window(const std::string& title, int width, int height, const std::string& meshFilePath) noexcept;

		void init();

		Geometry::Vector2D getMousePos() const noexcept;

		MouseButtonsState getMouseButtonsState() const noexcept;

		void setCallbacks() noexcept;
		static void onMouseMove(GLFWwindow* window, double xPos, double yPos) noexcept;
		static void onMouseButton(GLFWwindow* window, int button, int action, int mods) noexcept;
		static void onMouseScroll(GLFWwindow* window, double xOffset, double yOffset) noexcept;
		static void onKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods) noexcept;

	private:
		void chooseAction();
		void pan();

	private:
		std::string mTitle;
		std::string mMeshFilePath;
		int mWidth;
		int mHeight;

		MouseButtonsState mMouseButtonsState;
		Geometry::Vector2D mMousePos;

		GLFWwindow* mWindow;
		std::unique_ptr<Scene> mScene;
		std::unique_ptr<Viewport> mViewport;

		static std::unique_ptr<Window> sInstance;
	};
}

