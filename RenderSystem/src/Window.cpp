module;
#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"
#include <glfw/glfw3.h>
module Window;

import <iostream>;

import RenderSystemConsts;

namespace RenderSystem
{
	std::unique_ptr<Window> Window::sInstance {};

	Window::Window(const std::string& title, int width, int height, const std::string& meshFilePath) :
		mTitle(title),
		mMeshFilePath(meshFilePath),
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

		init();
	}

	void Window::init()
	{
		mViewport = std::make_unique<Viewport>(VIEWPORT_POSITION.x(), VIEWPORT_POSITION.y(), mWidth, mHeight);
		mScene = std::make_unique<Scene>(mMeshFilePath, this);
		mScene->adjust(mViewport->getFov());
		setCallbacks();
	}

	void Window::setCallbacks()
	{
		glfwSetCursorPosCallback(mWindow, onMouseMove);
		glfwSetMouseButtonCallback(mWindow, onMouseButton);
		glfwSetScrollCallback(mWindow, onMouseScroll);
		glfwSetKeyCallback(mWindow, onKey);
		glfwSetFramebufferSizeCallback(mWindow, onFramebufferSizeChanged);
	}

	Window* Window::createInstance(const std::string& title, int width, int height, const std::string& meshFilePath)
	{
		static auto window = new Window(title, width, height, meshFilePath);
		sInstance = std::move(std::unique_ptr<Window>(window));
		return sInstance.get();
	}

	Window* Window::getInstance()
	{
		return sInstance.get();
	}

	void Window::render()
	{
		if (!mWindow || !mScene)
		{
			throw std::exception("Window or RenderSystem are nullptr");
		}
		
		while (!glfwWindowShouldClose(mWindow))
		{
			mScene->render();
			glfwSwapBuffers(mWindow);
			glfwWaitEvents();
		}
	}

	const std::unique_ptr<Viewport>& Window::getViewport() const
	{
		return mViewport;
	}

	Geometry::Vector2D Window::getMousePos() const
	{
		double mousePosX, mousePosY;
		glfwGetCursorPos(mWindow, &mousePosX, &mousePosY);

		return { static_cast<float>(mousePosX), static_cast<float>(mousePosY) };
	}

	MouseButtonsState Window::getMouseButtonsState() const
	{
		MouseButtonsState mouseButtonsState;
		int leftMouseButtonState = glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT);
		mouseButtonsState.leftButtonPressed = (leftMouseButtonState == GLFW_PRESS);
		int rightMouseButtonState = glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_RIGHT);
		mouseButtonsState.rightButtonPressed = (rightMouseButtonState == GLFW_PRESS);
		int middleMouseButtonState = glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_MIDDLE);
		mouseButtonsState.middleButtonPressed = (middleMouseButtonState == GLFW_PRESS);

		return mouseButtonsState;
	}

	void Window::onMouseMove(GLFWwindow* window, double xPos, double yPos)
	{
		if (!sInstance || !window)
		{
			return;
		}

		sInstance->mMousePos = Geometry::Vector2D(static_cast<float>(xPos), static_cast<float>(yPos));
		sInstance->chooseAction();
	}

	void Window::chooseAction()
	{
		auto mouseButtonsState = sInstance->getMouseButtonsState();
		if (mouseButtonsState.middleButtonPressed)
			pan();
	}

	void Window::pan()
	{
		mScene->pan(unProject(mSavedMousePos), unProject(mMousePos));
		mSavedMousePos = mMousePos;
	}

	void Window::zoom(int yOffset)
	{
		mScene->zoomToPoint(unProject(getMousePos()), yOffset);
	}

	void Window::resizeViewport(int width, int height)
	{
		mViewport->resize(width, height);
		mScene->setProjectionMatrix(mViewport->getProjectionMatrix());
	}

	Geometry::Vector3D Window::unProject(const Geometry::Vector2D& mousePos) const
	{
		auto modelViewMatrix = mScene->getViewMatrix() * mScene->getModelMatrix();
		auto projectionMatrix = mViewport->getProjectionMatrix();
		auto viewportPos = mViewport->getPos();
		auto viewportHeight = static_cast<float>(mViewport->getHeight());
		Geometry::Vector4D viewport = { viewportPos.x(), viewportPos.y(), static_cast<float>(mViewport->getWidth()), viewportHeight };
		Geometry::Vector3D mousePos3D(mousePos.x(), viewportHeight - mousePos.y(), 0.0);

		return mousePos3D.unProject(modelViewMatrix, projectionMatrix, viewport);
	}

	void Window::onMouseButton(GLFWwindow* window, int button, int action, int mods)
	{
		if (!sInstance || !window)
			return;

		if (action == GLFW_PRESS)
		{
			sInstance->mSavedMousePos = sInstance->getMousePos();
		}
	}

	void Window::onMouseScroll(GLFWwindow* window, double xOffset, double yOffset)
	{
		if (!sInstance || !window)
			return;

		sInstance->zoom(static_cast<int>(yOffset));
	}

	void Window::onKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods)
	{
		if (!sInstance || !window)
			return;
	}

	void Window::onFramebufferSizeChanged(GLFWwindow* window, int width, int height)
	{
		if (!sInstance || !window)
			return;

		if (width == 0 || height == 0)
			return;

		sInstance->resizeViewport(width, height);
	}
}
