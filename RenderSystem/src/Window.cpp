module;
#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"
#include <glfw/glfw3.h>
#include "GeometryCore/Vector.h"
module Window;

import std;

import Scene;
import Viewport;
import Mesh;
import IEventHandler;

namespace RenderSystem
{
	std::unique_ptr<Window> Window::sInstance {};

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

		setCallbacks();
	}

	void Window::setCallbacks() noexcept
	{
		glfwSetCursorPosCallback(mWindow, onMouseMove);
		glfwSetMouseButtonCallback(mWindow, onMouseButton);
		glfwSetScrollCallback(mWindow, onMouseScroll);
		glfwSetKeyCallback(mWindow, onKey);
	}

	Window* Window::createInstance(const std::string& title, int width, int height) noexcept
	{
		static auto window = new Window(title, width, height);
		sInstance = std::move(std::unique_ptr<Window>(window));
		return sInstance.get();
	}

	Window* Window::getInstance() noexcept
	{
		return sInstance.get();
	}

	void Window::setScene(std::unique_ptr<Scene> scene) noexcept
	{
		mScene = std::move(scene);
		subscribe(mScene.get());
		mScene->setViewport(0, 0, mWidth, mHeight);
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
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			mScene->render();
			glfwSwapBuffers(mWindow);
			glfwWaitEvents();
		}
	}
	
	void Window::subscribe(IEventHandler* eventHandler) noexcept
	{
		mEventHandlers.push_back(eventHandler);
	}

	Geometry::Vector2D Window::getMousePos() const noexcept
	{
		double mousePosX, mousePosY;
		glfwGetCursorPos(mWindow, &mousePosX, &mousePosY);

		return { mousePosX, mousePosY };
	}

	void Window::onMouseMove(GLFWwindow* window, double xPos, double yPos) noexcept
	{
		if (!sInstance || !window)
		{
			return;
		}

		MouseButtonsState mouseButtonsState;
		int leftMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		mouseButtonsState.leftButtonPressed = (leftMouseButtonState == GLFW_PRESS);
		int rightMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		mouseButtonsState.rightButtonPressed = (rightMouseButtonState == GLFW_PRESS);
		int middleMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
		mouseButtonsState.middleButtonPressed = (middleMouseButtonState == GLFW_PRESS);

		Geometry::Vector2D mousePos(xPos, yPos);
		for (auto& eventHandler : sInstance->mEventHandlers)
		{
			eventHandler->onMouseMove(mousePos, mouseButtonsState);
		}
	}

	void Window::onMouseButton(GLFWwindow* window, int button, int action, int mods) noexcept
	{
		if (!sInstance || !window)
		{
			return;
		}

		auto mousePos = sInstance->getMousePos();
		if (action == GLFW_PRESS)
		{
			for (auto& eventHandler : sInstance->mEventHandlers)
			{
				eventHandler->onMouseDown(button, mousePos);
			}

			return;
		}

		for (auto& eventHandler : sInstance->mEventHandlers)
		{
			eventHandler->onMouseUp(button);
		}
	}

	void Window::onMouseScroll(GLFWwindow* window, double xOffset, double yOffset) noexcept
	{
		if (!sInstance || !window)
		{
			return;
		}

		for (auto& eventHandler : sInstance->mEventHandlers)
		{
			eventHandler->onMouseScroll(yOffset);
		}
	}

	void Window::onKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods) noexcept
	{
		if (!sInstance || !window)
		{
			return;
		}

		if (action == GLFW_PRESS)
		{
			for (auto& eventHandler : sInstance->mEventHandlers)
			{
				eventHandler->onKeyDown(keyCode);
			}

			return;
		}

		for (auto& eventHandler : sInstance->mEventHandlers)
		{
			eventHandler->onKeyUp(keyCode);
		}
	}
}
