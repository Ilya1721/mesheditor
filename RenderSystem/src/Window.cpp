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

	Window::Window(const std::string& title, int width, int height, const std::string& meshFilePath) noexcept :
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
		//mScene->adjustCamera(mViewport->getFov());
		setCallbacks();
	}

	void Window::setCallbacks() noexcept
	{
		glfwSetCursorPosCallback(mWindow, onMouseMove);
		glfwSetMouseButtonCallback(mWindow, onMouseButton);
		glfwSetScrollCallback(mWindow, onMouseScroll);
		glfwSetKeyCallback(mWindow, onKey);
	}

	Window* Window::createInstance(const std::string& title, int width, int height, const std::string& meshFilePath) noexcept
	{
		static auto window = new Window(title, width, height, meshFilePath);
		sInstance = std::move(std::unique_ptr<Window>(window));
		return sInstance.get();
	}

	Window* Window::getInstance() noexcept
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

	const std::unique_ptr<Viewport>& Window::getViewport() const noexcept
	{
		return mViewport;
	}

	Geometry::Vector2D Window::getMousePos() const noexcept
	{
		double mousePosX, mousePosY;
		glfwGetCursorPos(mWindow, &mousePosX, &mousePosY);

		return { static_cast<float>(mousePosX), static_cast<float>(mousePosY) };
	}

	MouseButtonsState Window::getMouseButtonsState() const noexcept
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

	void Window::onMouseMove(GLFWwindow* window, double xPos, double yPos) noexcept
	{
		if (!sInstance || !window)
		{
			return;
		}

		sInstance->mMouseButtonsState = sInstance->getMouseButtonsState();
		sInstance->mMousePos = Geometry::Vector2D(static_cast<float>(xPos), static_cast<float>(yPos));

		sInstance->chooseAction();
	}

	void Window::chooseAction()
	{
		if (mMouseButtonsState.middleButtonPressed)
			pan();
	}

	void Window::pan()
	{
		/*auto viewportHeight = mViewport->getHeight();
		auto viewportWidth = mViewport->getWidth();

		auto& camera = mScene->getCamera();
		const auto& modelViewMatrix = camera.getViewMatrix();
		const auto& projectionMatrix = mViewport->getProjectionMatrix();
		Geometry::Vector4D viewportDimensions { 0.0, 0.0, static_cast<double>(viewportWidth), static_cast<double>(viewportHeight) };

		const auto& projectedCenter = Geometry::Vector3D::project({ 0.0, 0.0, 0.0 }, modelViewMatrix, projectionMatrix, viewportDimensions);
		Geometry::Vector3D startScreenPos(startMousePos.x(), viewportHeight - startMousePos.y(), projectedCenter.z());
		Geometry::Vector3D endScreenPos(endMousePos.x(), viewportHeight - endMousePos.y(), projectedCenter.z());

		auto unProjectedStartMousePos = Geometry::Vector3D::unProject(startScreenPos, modelViewMatrix, projectionMatrix, viewportDimensions);
		auto unProjectedEndMousePos = Geometry::Vector3D::unProject(endScreenPos, modelViewMatrix, projectionMatrix, viewportDimensions);
		auto movement = unProjectedEndMousePos - unProjectedStartMousePos;

		camera.translate(movement);*/
	}

	void Window::onMouseButton(GLFWwindow* window, int button, int action, int mods) noexcept
	{
		if (!sInstance || !window)
		{
			return;
		}
	}

	void Window::onMouseScroll(GLFWwindow* window, double xOffset, double yOffset) noexcept
	{
		if (!sInstance || !window)
		{
			return;
		}
	}

	void Window::onKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods) noexcept
	{
		if (!sInstance || !window)
		{
			return;
		}
	}
}
