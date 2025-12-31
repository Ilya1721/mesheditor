#pragma once

#include <glfw/glfw3.h>

#include <memory>
#include <string>

#include "Constants.h"
#include "GeometryCore/Typedefs.h"
#include "MeshCore/Intersection.h"
#include "Object3DIntersection.h"
#include "Viewport.h"
#include "ViewportListener.h"

namespace RenderSystem
{
  using namespace GeometryCore;
  using namespace MeshCore;

  class Scene;
  class OperationsDispatcher;
  class Object3D;

  enum class MouseButtonPressed
  {
    LEFT,
    RIGHT,
    MIDDLE,
    NONE
  };

  class Window
  {
   public:
    Window(int width, int height, const std::string& meshFilePath);
    ~Window();

    Point2D getCursorPos() const;
    Point3D unProject(const Point2D& cursorPos, float depth = DEFAULT_Z_VALUE) const;
    Point3D screenCoordinatesToNDC(
      const Point2D& cursorPos, float depth = DEFAULT_Z_VALUE
    ) const;
    bool isMouseButtonPressed(int button) const;
    bool isKeyPressed(int key) const;
    Object3DIntersection getCursorSceneIntersection(
      IntersectionMode intersectionMode = IntersectionMode::SURFACE
    );

    void render();

    void onMouseMove(double cursorX, double cursorY);
    void onMouseButton(int button, int action, int mods);
    void onMouseScroll(double xOffset, double yOffset);
    void onFramebufferSizeChanged(int width, int height);
    void onKey(int key, int scancode, int action, int mods);

   private:
    void initLibs(int windowWidth, int windowHeight);
    void init(const std::string& meshFilePath, int windowWidth, int windowHeight);
    void resizeViewport(int width, int height);
    void setCallbacks();
    void onViewportChanged();
    void addViewportListeners();
    Ray castCursorRay() const;

   private:
    GLFWwindow* mWindow;
    std::unique_ptr<Scene> mScene;
    std::unique_ptr<Viewport> mViewport;
    std::unique_ptr<OperationsDispatcher> mOperationsDispatcher;
    std::vector<ViewportListener*> mViewportListeners;
    Point2D mSavedCursorPosition;
  };
}  // namespace RenderSystem
