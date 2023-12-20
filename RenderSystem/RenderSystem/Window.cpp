#include "Window.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"
#include <glfw/glfw3.h>

#include <glm/ext/matrix_projection.hpp>

#include <iostream>

#include "Constants.h"

namespace RenderSystem
{
	std::unique_ptr<Window> Window::sInstance {};

	Window::Window(const std::string& title, int width, int height, const std::string& meshFilePath) :
		mTitle(title),
		mMeshFilePath(meshFilePath),
		mWidth(width),
		mHeight(height),
		mMouseButtonPressed(MouseButtonPressed::NONE)
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
		mViewport = std::make_unique<Viewport>(VIEWPORT_POSITION.x, VIEWPORT_POSITION.y, mWidth, mHeight);
		mViewport->setProjectionType(PROJECTION_TYPE::PERSPECTIVE);
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

	glm::vec2 Window::getMousePos() const
	{
		double mousePosX, mousePosY;
		glfwGetCursorPos(mWindow, &mousePosX, &mousePosY);
		return { mousePosX, mousePosY };
	}

	void Window::onMouseMove(GLFWwindow* window, double cursorX, double cursorY)
	{
		glm::vec2 currentCursorPosition(cursorX, cursorY);

		switch (sInstance->mMouseButtonPressed)
		{
		case MouseButtonPressed::MIDDLE:
			sInstance->mScene->pan(sInstance->unProject(sInstance->mSavedCursorPosition), sInstance->unProject(currentCursorPosition));
			break;
		case MouseButtonPressed::LEFT:
			sInstance->mScene->orbit(sInstance->mousePosToNDC(sInstance->mSavedCursorPosition), sInstance->mousePosToNDC(currentCursorPosition));
			break;
		}

		if (sInstance->mMouseButtonPressed != MouseButtonPressed::NONE)
		{
			sInstance->mSavedCursorPosition = currentCursorPosition;
		}
	}

	void Window::resizeViewport(int width, int height)
	{
		mViewport->resize(width, height);
		mScene->setProjectionMatrix(mViewport->getProjectionMatrix());
	}

	glm::vec3 Window::unProject(const glm::vec2& mousePos) const
	{
		const auto& viewMatrix = mScene->getViewMatrix();
		const auto& projectionMatrix = mViewport->getProjectionMatrix();
		const auto& viewportPos = mViewport->getPos();
		auto viewportHeight = mViewport->getHeight();
		glm::vec4 viewport = { viewportPos.x, viewportPos.y, mViewport->getWidth(), viewportHeight };
		glm::vec3 mousePos3D(mousePos.x, viewportHeight - mousePos.y, 0.0);
		return glm::unProject(mousePos3D, viewMatrix, projectionMatrix, viewport);
	}

	glm::vec3 Window::mousePosToNDC(const glm::vec2& mousePos) const
	{
		auto ndcX = 2.0f * mousePos.x / mWidth - 1.0f;
		auto ndcY = 1.0f - 2.0f * mousePos.y / mHeight;
		return glm::vec3(ndcX, ndcY, 0.0f);
	}

	void Window::onMouseButton(GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_RELEASE)
		{
			sInstance->mMouseButtonPressed = MouseButtonPressed::NONE;
			return;
		}

		if (action != GLFW_PRESS)
			return;

		switch (button)
		{
			case GLFW_MOUSE_BUTTON_LEFT:
				sInstance->mMouseButtonPressed = MouseButtonPressed::LEFT;
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				sInstance->mMouseButtonPressed = MouseButtonPressed::RIGHT;
				break;
			case GLFW_MOUSE_BUTTON_MIDDLE:
				sInstance->mMouseButtonPressed = MouseButtonPressed::MIDDLE;
				break;
		}

		sInstance->mSavedCursorPosition = sInstance->getMousePos();
	}

	void Window::onMouseScroll(GLFWwindow* window, double xOffset, double yOffset)
	{
		sInstance->mScene->zoomToPoint(sInstance->unProject(sInstance->getMousePos()), yOffset);
	}

	void Window::onKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods)
	{

	}

	void Window::onFramebufferSizeChanged(GLFWwindow* window, int width, int height)
	{
		if (width == 0 || height == 0)
			return;

		sInstance->resizeViewport(width, height);
	}
}
