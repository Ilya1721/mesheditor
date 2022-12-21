#include "pch.h"

#include "Window.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

namespace RenderSystem
{
	Window::Window(const std::string& title, int width, int height) noexcept :
		mTitle(title),
		mWidth(width),
		mHeight(height)
	{
		glfwInit();

		mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

		if (!mWindow)
		{
			std::cerr << "glfw create window returned nullptr" << std::endl;
			glfwTerminate();
		}

		glfwMakeContextCurrent(mWindow);

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			mWindow = nullptr;
			glfwTerminate();
			std::cerr << "Failed to init glad" << std::endl;
		}
	}

	void Window::setScene(std::unique_ptr<Scene> scene) noexcept
	{
		mScene = std::move(scene);
	}

	void Window::start() noexcept
	{
		if (!mWindow || !mScene)
		{
			std::cerr << "Window or RenderSystem are nullptr" << std::endl;
			return;
		}

		mScene->init();
		
		while (!glfwWindowShouldClose(mWindow))
		{
			mScene->render();
			glfwSwapBuffers(mWindow);
			glfwWaitEvents();
		}
	}
}
