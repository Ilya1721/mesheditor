#include "Window.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glfw/glfw3.h>

#include <iostream>

#include "Constants.h"
#include "GeometryCore/Ray.h"
#include "OperationsDispatcher.h"
#include "Scene.h"
#include "Viewport.h"
#include "glad/glad.h"

namespace
{
  using namespace RenderSystem;

  Window* instance = nullptr;

  void onMouseMove([[maybe_unused]] GLFWwindow* window, double cursorX, double cursorY)
  {
    instance->onMouseMove(cursorX, cursorY);
  }

  void onMouseButton(
    [[maybe_unused]] GLFWwindow* window, int button, int action, int mods
  )
  {
    instance->onMouseButton(button, action, mods);
  }

  void onMouseScroll([[maybe_unused]] GLFWwindow* window, double xOffset, double yOffset)
  {
    instance->onMouseScroll(xOffset, yOffset);
  }

  void onKey(
    [[maybe_unused]] GLFWwindow* window, int key, int scancode, int action, int mods
  )
  {
    instance->onKey(key, scancode, action, mods);
  }

  void onFramebufferSizeChanged(
    [[maybe_unused]] GLFWwindow* window, int width, int height
  )
  {
    instance->onFramebufferSizeChanged(width, height);
  }
}  // namespace

namespace RenderSystem
{
  Window::Window(int width, int height, const std::string& meshFilePath)
  {
    instance = this;
    initLibs(width, height);
    init(meshFilePath, width, height);
  }

  Window::~Window()
  {
    glfwDestroyWindow(mWindow);
    glfwTerminate();
  }

  void Window::initLibs(int windowWidth, int windowHeight)
  {
    glfwInit();
    mWindow =
      glfwCreateWindow(windowWidth, windowHeight, WINDOW_TITLE.c_str(), nullptr, nullptr);
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

  void Window::init(const std::string& meshFilePath, int windowWidth, int windowHeight)
  {
    mScene = std::make_unique<Scene>(
      meshFilePath, static_cast<float>(windowWidth) / windowHeight
    );
    mViewport = std::make_unique<Viewport>(
      windowWidth, windowHeight, &mScene->getRootObject().getBBox()
    );
    mOperationsDispatcher = std::make_unique<OperationsDispatcher>(
      this, mViewport.get(), mScene.get(), mScene->getCamera()
    );
    addViewportListeners();
    setCallbacks();
    resizeViewport(windowWidth, windowHeight);
  }

  void Window::setCallbacks()
  {
    glfwSetCursorPosCallback(mWindow, ::onMouseMove);
    glfwSetMouseButtonCallback(mWindow, ::onMouseButton);
    glfwSetScrollCallback(mWindow, ::onMouseScroll);
    glfwSetFramebufferSizeCallback(mWindow, ::onFramebufferSizeChanged);
    glfwSetKeyCallback(mWindow, ::onKey);
    mViewport->addOnViewportChangedCallback([this]() { onViewportChanged(); });
  }

  void Window::onViewportChanged()
  {
    for (auto& listener : mViewportListeners)
    {
      listener->onViewportChanged(mViewport.get());
    }
  }

  void Window::addViewportListeners()
  {
    auto sceneListeners = mScene->getViewportListeners();
    mViewportListeners.insert(
      mViewportListeners.end(), sceneListeners.begin(), sceneListeners.end()
    );
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

  Point2D Window::getCursorPos() const
  {
    double x, y;
    glfwGetCursorPos(mWindow, &x, &y);
    return {x, y};
  }

  void Window::resizeViewport(int width, int height)
  {
    if (width > 0 && height > 0) { mViewport->resize(width, height); }
  }

  Point3D Window::unProject(const Point2D& cursorPos, float depth) const
  {
    glm::vec4 viewportData = {
      mViewport->getPos().x, mViewport->getPos().y, mViewport->getWidth(),
      mViewport->getHeight()
    };
    Point3D cursorPosGL3D(cursorPos.x, mViewport->getHeight() - cursorPos.y, depth);
    return mScene->unProject(
      cursorPosGL3D, mViewport->getProjectionMatrix(), viewportData
    );
  }

  Point3D Window::screenCoordinatesToNDC(const Point2D& cursorPos, float depth) const
  {
    Point3D ndcPos {};
    ndcPos.x = (2.0f * cursorPos.x) / mViewport->getWidth() - 1.0f;
    ndcPos.y =
      2.0f * (mViewport->getHeight() - cursorPos.y) / mViewport->getHeight() - 1.0f;
    ndcPos.z = depth;

    return ndcPos;
  }

  bool Window::isMouseButtonPressed(int button) const
  {
    return glfwGetMouseButton(mWindow, button) == GLFW_PRESS;
  }

  bool Window::isKeyPressed(int key) const
  {
    return glfwGetKey(mWindow, key) == GLFW_PRESS;
  }

  Object3DIntersection Window::getCursorSceneIntersection(
    IntersectionMode intersectionMode
  )
  {
    return mScene->getRayIntersection(castCursorRay(), intersectionMode);
  }

  Ray Window::castCursorRay() const
  {
    auto nearCursorPosInWorldSpace = unProject(getCursorPos(), 0.0f);
    auto farCursorPosInWorldSpace = unProject(getCursorPos(), 1.0f);
    return {
      nearCursorPosInWorldSpace, farCursorPosInWorldSpace - nearCursorPosInWorldSpace
    };
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

  void Window::onKey(
    int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods
  )
  {
    if (action == GLFW_PRESS) { mOperationsDispatcher->onKeyPressed(key); }
  }
}  // namespace RenderSystem
