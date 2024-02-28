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

	Window::Window(int width, int height, const std::string& meshFilePath) :
		mWidth(width),
		mHeight(height),
		mMouseButtonPressed(MouseButtonPressed::NONE)
	{
		initGLFW();
		initScene(meshFilePath);
		setCallbacks();
	}

	void Window::initGLFW()
	{
		glfwInit();
		mWindow = glfwCreateWindow(mWidth, mHeight, WINDOW_TITLE.c_str(), nullptr, nullptr);
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

	void Window::initScene(const std::string& meshFilePath)
	{
		mViewport = std::make_unique<Viewport>(VIEWPORT_POSITION.x, VIEWPORT_POSITION.y, mWidth, mHeight);
		mScene = std::make_unique<Scene>(meshFilePath, this);
		mOperationsDispatcher = std::make_unique<OperationsDispatcher>(mScene.get());
	}

	void Window::setCallbacks()
	{
		glfwSetCursorPosCallback(mWindow, onMouseMove);
		glfwSetMouseButtonCallback(mWindow, onMouseButton);
		glfwSetScrollCallback(mWindow, onMouseScroll);
		glfwSetFramebufferSizeCallback(mWindow, onFramebufferSizeChanged);
		glfwSetKeyCallback(mWindow, onKey);
	}

	Window* Window::createInstance(int width, int height, const std::string& meshFilePath)
	{
		sInstance = std::move(std::unique_ptr<Window>(new Window(width, height, meshFilePath)));
		return sInstance.get();
	}

	void Window::render()
	{		
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

	glm::vec2 Window::getCursorPos() const
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
			sInstance->mScene->orbit(sInstance->screenCoordinatesToNDC(sInstance->mSavedCursorPosition), sInstance->screenCoordinatesToNDC(currentCursorPosition));
			break;
		}

		sInstance->mOperationsDispatcher->onMouseMove(sInstance->mSavedCursorPosition, currentCursorPosition);

		if (sInstance->mMouseButtonPressed != MouseButtonPressed::NONE)
		{
			sInstance->mSavedCursorPosition = currentCursorPosition;
		}
	}

	void Window::resizeViewport(int width, int height)
	{
		if (width == 0 || height == 0)
			return;

		mViewport->resize(width, height);
		mScene->setProjectionMatrix(mViewport->getProjectionMatrix());
	}

	glm::vec3 Window::unProject(const glm::vec2& cursorPos) const
	{
		glm::vec4 viewportData = { mViewport->getPos().x, mViewport->getPos().y, mViewport->getWidth(), mViewport->getHeight() };
		glm::vec3 cursorPosGL3D(cursorPos.x, mViewport->getHeight() - cursorPos.y, 0.0);
		return glm::unProject(cursorPosGL3D, mScene->getViewMatrix(), mViewport->getProjectionMatrix(), viewportData);
	}

	glm::vec3 Window::unProjectToCameraTargetPlane(const glm::vec2& cursorPos) const
	{
		return sInstance->mScene->getCamera().projectToTargetPlane(sInstance->unProject(cursorPos));
	}

	glm::vec3 Window::screenCoordinatesToNDC(const glm::vec2& cursorPos) const
	{
		glm::vec3 ndcPos{};
		ndcPos.x = (2.0 * cursorPos.x) / mViewport->getWidth() - 1.0;
		ndcPos.y = (mViewport->getHeight() - cursorPos.y) / mViewport->getHeight() * 2.0 - 1.0;
		ndcPos.z = 0.0;

		return ndcPos;
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

		sInstance->mSavedCursorPosition = sInstance->getCursorPos();

		switch (button)
		{
			case GLFW_MOUSE_BUTTON_LEFT:
				sInstance->mOperationsDispatcher->onMouseClick(sInstance->mSavedCursorPosition);
				sInstance->mMouseButtonPressed = MouseButtonPressed::LEFT;
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				sInstance->mMouseButtonPressed = MouseButtonPressed::RIGHT;
				break;
			case GLFW_MOUSE_BUTTON_MIDDLE:
				sInstance->mMouseButtonPressed = MouseButtonPressed::MIDDLE;
				break;
		}
	}

	void Window::onMouseScroll(GLFWwindow* window, double xOffset, double yOffset)
	{
		sInstance->mScene->zoom(yOffset);
	}

	void Window::onFramebufferSizeChanged(GLFWwindow* window, int width, int height)
	{
		sInstance->resizeViewport(width, height);
	}

	void Window::onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			sInstance->mOperationsDispatcher->toggle(key);
		}
	}
}
