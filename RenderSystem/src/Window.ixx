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
		static Window* createInstance(const std::string& title, int width, int height, const std::string& meshFilePath);
		static Window* getInstance();

		void render();

		const std::unique_ptr<Viewport>& getViewport() const;

	private:
		Window(const std::string& title, int width, int height, const std::string& meshFilePath);

		void init();

		Geometry::Vector2D getMousePos() const;

		MouseButtonsState getMouseButtonsState() const;

		void setCallbacks();
		static void onMouseMove(GLFWwindow* window, double xPos, double yPos);
		static void onMouseButton(GLFWwindow* window, int button, int action, int mods);
		static void onMouseScroll(GLFWwindow* window, double xOffset, double yOffset);
		static void onKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods);
		static void onFramebufferSizeChanged(GLFWwindow* window, int width, int height);

	private:
		void chooseAction();
		void pan();
		void resizeViewport(int width, int height);

	private:
		std::string mTitle;
		std::string mMeshFilePath;
		int mWidth;
		int mHeight;

		MouseButtonsState mMouseButtonsState;
		Geometry::Vector2D mSavedMousePos;
		Geometry::Vector2D mMousePos;

		GLFWwindow* mWindow;
		std::unique_ptr<Scene> mScene;
		std::unique_ptr<Viewport> mViewport;

		static std::unique_ptr<Window> sInstance;
	};
}

