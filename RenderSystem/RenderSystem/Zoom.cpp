#include "Zoom.h"

#include "Window.h"

namespace RenderSystem
{
  Zoom::Zoom(Window* window) : Operation(window) {}

  void Zoom::onMouseScroll(double offset) { mWindow->zoom(offset); }
}  // namespace RenderSystem