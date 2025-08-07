#include "RequiresBindBeforeInvoke.h"

namespace RenderSystem
{
  void RequiresBindBeforeInvoke::invoke(const std::function<void()>& action)
  {
    bind();
    action();
    unbind();
  }
}  // namespace RenderSystem
