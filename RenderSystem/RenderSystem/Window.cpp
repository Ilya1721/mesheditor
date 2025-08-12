#include "Window.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glfw/glfw3.h>

#include <glm/ext/matrix_projection.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Constants.h"
#include "GeometryCore/Ray.h"
#include "GeometryCore/Transforms.h"
#include "OperationsDispatcher.h"
#include "Scene.h"
#include "Viewport.h"
#include "glad.h"

namespace
{
  using namespace RenderSystem;

  const std::string DEPTH_MAP_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/DepthMapVertexShader.vert)";
  const std::string DEPTH_MAP_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/DepthMapFragmentShader.frag)";
  const std::string SCENE_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/VertexShader.vert)";
  const std::string SCENE_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/FragmentShader.frag)";

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
    : mWidth(width), mHeight(height)
  {
    instance = this;
    initGLFW();
    init(meshFilePath);
    adjustCamera();
    adjustDirLightSourcePos();
    setCallbacks();
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

  void Window::init(const std::string& meshFilePath)
  {
    mSceneShaderProgram = std::make_unique<SceneShaderProgram>(
      SCENE_VERTEX_SHADER_PATH, SCENE_FRAGMENT_SHADER_PATH
    );
    mRenderer = std::make_unique<Renderer>(mSceneShaderProgram.get());
    mShadowController = std::make_unique<ShadowController>(
      DEPTH_MAP_VERTEX_SHADER_PATH, DEPTH_MAP_FRAGMENT_SHADER_PATH
    );
    auto aspectRatio = static_cast<float>(mWidth) / mHeight;
    mScene = std::make_unique<Scene>(
      meshFilePath, mRenderer.get(), mShadowController.get(), mSceneShaderProgram.get(),
      aspectRatio
    );
    mCamera = std::make_unique<Camera>(mSceneShaderProgram.get());
    mViewport = std::make_unique<Viewport>(
      mWidth, mHeight, &mScene->getRootObject().getBBox(), mSceneShaderProgram.get()
    );
    mOperationsDispatcher = std::make_unique<OperationsDispatcher>(this);
    resizeViewport(mWidth, mHeight);
  }

  void Window::setCallbacks()
  {
    glfwSetCursorPosCallback(mWindow, ::onMouseMove);
    glfwSetMouseButtonCallback(mWindow, ::onMouseButton);
    glfwSetScrollCallback(mWindow, ::onMouseScroll);
    glfwSetFramebufferSizeCallback(mWindow, ::onFramebufferSizeChanged);
    glfwSetKeyCallback(mWindow, ::onKey);
  }

  void Window::adjustCamera()
  {
    const auto& projectionType = mViewport->getProjectionType();
    const auto& sceneBBox = mScene->getRootObject().getBBox();
    const auto& fov = mViewport->getFov();
    auto cameraPosInCameraSpace = mCamera->adjust(projectionType, sceneBBox, fov);
    mSceneShaderProgram->setCameraPos(cameraPosInCameraSpace);
  }

  void Window::adjustDirLightSourcePos()
  {
    const auto dirLightPos =
      Point3D(LIGHT_SOURCE_POS_X, LIGHT_SOURCE_POS_Y, LIGHT_SOURCE_POS_Z);
    Point3D dirLightSourcePosInCameraSpace =
      transformPoint(dirLightPos, mCamera->getViewMatrix());
    mScene->setDirLightSourcePos(dirLightSourcePosInCameraSpace);
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
    if (width <= 0 || height <= 0) { return; }

    mWidth = width;
    mHeight = height;

    mViewport->resize(width, height);
    mSceneShaderProgram->setProjection(mViewport->getProjectionMatrix());
    mShadowController->setTextureDimensions(width, height);
    mScene->setAspectRatio(static_cast<float>(width) / height);
  }

  Point3D Window::unProject(const Point2D& cursorPos, float depth) const
  {
    glm::vec4 viewportData = {
      mViewport->getPos().x, mViewport->getPos().y, mViewport->getWidth(),
      mViewport->getHeight()
    };
    Point3D cursorPosGL3D(cursorPos.x, mViewport->getHeight() - cursorPos.y, depth);
    return glm::unProject(
      cursorPosGL3D, mCamera->getViewMatrix(), mViewport->getProjectionMatrix(),
      viewportData
    );
  }

  Point3D Window::screenCoordinatesToNDC(const Point2D& cursorPos, float depth) const
  {
    Point3D ndcPos {};
    ndcPos.x = (2.0 * cursorPos.x) / mViewport->getWidth() - 1.0;
    ndcPos.y =
      2.0 * (mViewport->getHeight() - cursorPos.y) / mViewport->getHeight() - 1.0;
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

  PROJECTION_TYPE Window::getProjectionType() const
  {
    return mViewport->getProjectionType();
  }

  float Window::getFov() const { return mViewport->getFov(); }

  Object3D* Window::getPickedObject() const { return mScene->getPickedObject(); }

  Point3D Window::projectToCameraTargetPlane(const Point3D& cursorPosInWorldSpace) const
  {
    return mCamera->projectToTargetPlane(cursorPosInWorldSpace);
  }

  bool Window::isCameraMovementEnabled() const { return mCamera->isMovementEnabled(); }

  Object3DIntersection Window::getIntersection(IntersectionMode intersectionMode)
  {
    return mScene->getIntersection(castCursorRay(), intersectionMode);
  }

  Ray Window::castCursorRay() const
  {
    auto nearCursorPosInWorldSpace = unProject(getCursorPos(), 0.0f);
    auto farCursorPosInWorldSpace = unProject(getCursorPos(), 1.0f);
    return {
      nearCursorPosInWorldSpace, farCursorPosInWorldSpace - nearCursorPosInWorldSpace
    };
  }

  Point3D Window::getDefaultPointLightSourcePos() const
  {
    return mScene->getDefaultPointLightSourcePos();
  }

  void Window::setPickedObject(Object3D* pickedObject)
  {
    mScene->setPickedObject(pickedObject);
  }

  void Window::toggleWireframe() { mScene->toggleWireframe(); }

  void Window::highlightWholeObject(const Object3D* object)
  {
    mScene->highlightWholeObject(object);
  }

  void Window::setHighlightedFacesData(const HighlightedFacesData& data)
  {
    mScene->setHighlightedFacesData(data);
  }

  void Window::addPointLight(
    const PointLightParams& params, const Point3D& lightSourcePos
  )
  {
    Point3D lightSourcePosInCameraSpace =
      transformPoint(lightSourcePos, mCamera->getViewMatrix());
    mScene->addPointLight(params, lightSourcePosInCameraSpace);
  }

  void Window::removePointLight(unsigned int index) { mScene->removePointLight(index); }

  void Window::setPointLightParams(unsigned int index, const PointLightParams& params)
  {
    mScene->setPointLightParams(index, params);
  }

  void Window::setPointLightSourcePos(unsigned int index, const Point3D& lightSourcePos)
  {
    mScene->setPointLightSourcePos(index, lightSourcePos);
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

  void Window::zoom(float step)
  {
    if (mViewport->getProjectionType() == PROJECTION_TYPE::PERSPECTIVE)
    {
      mCamera->zoom(
        step * mScene->getRootObject().getBBox().getHeight() * ZOOM_STEP_COEF
      );
    }
    else { mViewport->zoom(-step * ORTHO_ZOOM_STEP); }
  }

  void Window::smoothOrbit(float xOffset, float yOffset)
  {
    mCamera->smoothOrbit(xOffset, yOffset);
  }

  void Window::pan(
    const Point3D& startPointInWorldSpace,
    const Point3D& endPointInWorldSpace,
    PROJECTION_TYPE projectionType
  )
  {
    mCamera->pan(startPointInWorldSpace, endPointInWorldSpace, projectionType);
  }

  void Window::enableCameraMovement(bool isEnabled)
  {
    mCamera->enableMovement(isEnabled);
  }
}  // namespace RenderSystem
