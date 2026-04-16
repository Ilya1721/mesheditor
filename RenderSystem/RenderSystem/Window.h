#pragma once

#include <glfw/glfw3.h>

#include <memory>
#include <string>

#include "Constants.h"
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

    glm::vec2 getCursorPos() const;
    glm::vec3 unProject(const glm::vec2& cursorPos, float depth = DEFAULT_Z_VALUE) const;
    glm::vec3 screenCoordinatesToNDC(
      const glm::vec2& cursorPos, float depth = DEFAULT_Z_VALUE
    ) const;
    bool isMouseButtonPressed(int button) const;
    bool isKeyPressed(int key) const;
    Ray castCursorRay() const;

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

   private:
    GLFWwindow* mWindow;
    std::unique_ptr<Scene> mScene;
    std::unique_ptr<Viewport> mViewport;
    std::unique_ptr<OperationsDispatcher> mOperationsDispatcher;
    std::vector<ViewportListener*> mViewportListeners;
    glm::vec2 mSavedCursorPosition;
  };
}  // namespace RenderSystem
