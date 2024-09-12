#include "Window.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"
#include <glfw/glfw3.h>

#include <glm/ext/matrix_projection.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Constants.h"
#include "Scene.h"
#include "Viewport.h"
#include "OperationsDispatcher.h"

namespace RenderSystem
{
	Window Window::sInstance;
	Window* instancePtr = &sInstance;
}

namespace
{
	using namespace RenderSystem;

	void onMouseMove([[maybe_unused]] GLFWwindow* window, double cursorX, double cursorY)
	{
		instancePtr->onMouseMove(cursorX, cursorY);
	}

	void onMouseButton([[maybe_unused]] GLFWwindow* window, int button, int action, int mods)
	{
		instancePtr->onMouseButton(button, action, mods);
	}

	void onMouseScroll([[maybe_unused]] GLFWwindow* window, double xOffset, double yOffset)
	{
		instancePtr->onMouseScroll(xOffset, yOffset);
	}

	void onKey([[maybe_unused]] GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		instancePtr->onKey(key, scancode, action, mods);
	}

	void onFramebufferSizeChanged([[maybe_unused]] GLFWwindow* window, int width, int height)
	{
		instancePtr->onFramebufferSizeChanged(width, height);
	}
}

namespace RenderSystem
{
	Window& Window::getInstance()
	{
		return sInstance;
	}

	void Window::init(int width, int height, const std::string& meshFilePath)
	{
		mWidth = width;
		mHeight = height;
		initGLFW();
		setGLFWCallbacks();
		mInitializationCallbackMechanism.invokeCallbacks();
	}

	Window::~Window()
	{
		glfwDestroyWindow(mWindow);
		glfwTerminate();
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
			std::cerr << "Failed to init glad" << std::endl;
			mWindow = nullptr;
			glfwTerminate();
		}
	}

	void Window::initSceneAndViewport(const std::string& meshFilePath)
	{
		mViewport = std::make_unique<Viewport>(mWidth, mHeight, &Scene::getRootObject().getBBox(), shaderTransformationSystemPtr);
		mScene->adjustCameraAndLight();
		mOperationsDispatcher = std::make_unique<OperationsDispatcher>(mScene.get());
	}

	void Window::setGLFWCallbacks()
	{
		glfwSetCursorPosCallback(mWindow, ::onMouseMove);
		glfwSetMouseButtonCallback(mWindow, ::onMouseButton);
		glfwSetScrollCallback(mWindow, ::onMouseScroll);
		glfwSetFramebufferSizeCallback(mWindow, ::onFramebufferSizeChanged);
		glfwSetKeyCallback(mWindow, ::onKey);
	}

	void Window::render()
	{		
		while (!glfwWindowShouldClose(mWindow))
		{
			mScene->getRenderer().render();
			glfwSwapBuffers(mWindow);
			glfwWaitEvents();
		}
	}

	const std::unique_ptr<Scene>& Window::getScene() const
	{
		return mScene;
	}

	const std::unique_ptr<Viewport>& Window::getViewport() const
	{
		return mViewport;
	}

	Point2D Window::getCursorPos() const
	{
		double x, y;
		glfwGetCursorPos(mWindow, &x, &y);
		return { x, y };
	}

	void Window::resizeViewport(int width, int height)
	{
		if (width <= 0 || height <= 0)
		{
			return;
		}

		mViewport->resize(width, height);
		mScene->getRenderer().getShaderTransformationSystem().setProjection(glm::value_ptr(mViewport->getProjectionMatrix()));
	}

	Point3D Window::unProject(const Point2D& cursorPos, float depth) const
	{
		glm::vec4 viewportData = { mViewport->getPos().x, mViewport->getPos().y, mViewport->getWidth(), mViewport->getHeight() };
		Point3D cursorPosGL3D(cursorPos.x, mViewport->getHeight() - cursorPos.y, depth);
		return glm::unProject(cursorPosGL3D, mScene->getCamera().getViewMatrix(), mViewport->getProjectionMatrix(), viewportData);
	}

	Point3D Window::screenCoordinatesToNDC(const Point2D& cursorPos, float depth) const
	{
		Point3D ndcPos{};
		ndcPos.x = (2.0 * cursorPos.x) / mViewport->getWidth() - 1.0;
		ndcPos.y = 2.0 * (mViewport->getHeight() - cursorPos.y) / mViewport->getHeight() - 1.0;
		ndcPos.z = depth;

		return ndcPos;
	}

	Ray Window::castCursorRay() const
	{
		auto nearCursorPosInWorldSpace = unProject(getCursorPos(), 0.0f);
		auto farCursorPosInWorldSpace = unProject(getCursorPos(), 1.0f);

		return { nearCursorPosInWorldSpace, farCursorPosInWorldSpace - nearCursorPosInWorldSpace };
	}

	bool Window::isMouseButtonPressed(int button) const
	{
		return glfwGetMouseButton(mWindow, button) == GLFW_PRESS;
	}

	bool Window::isKeyPressed(int key) const
	{
		return glfwGetKey(mWindow, key) == GLFW_PRESS;
	}

	int Window::getWidth() const
	{
		return mWidth;
	}

	int Window::getHeight() const
	{
		return mHeight;
	}

	void Window::onMouseMove(double cursorX, double cursorY)
	{
		Point2D currentCursorPosition(cursorX, cursorY);
		mOperationsDispatcher->onMouseMove(mSavedCursorPosition, currentCursorPosition);
		mSavedCursorPosition = currentCursorPosition;
	}

	void Window::onMouseButton(int button, int action, [[maybe_unused]] int mods)
	{
		if (action == GLFW_PRESS)
		{
			mSavedCursorPosition = getCursorPos();
			mOperationsDispatcher->onMouseClick();
		}
	}

	void Window::onMouseScroll([[maybe_unused]] double xOffset, double yOffset)
	{
		mOperationsDispatcher->onMouseScroll(yOffset);
	}

	void Window::onFramebufferSizeChanged(int width, int height)
	{
		resizeViewport(width, height);
	}

	void Window::onKey(int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods)
	{
		if (action == GLFW_PRESS)
		{
			mOperationsDispatcher->onKeyPressed(key);
		}
	}

	void Window::addWindowInitializedCallback(const std::function<void()>& callback)
	{
		mInitializationCallbackMechanism.addCallback(callback);
	}
}
