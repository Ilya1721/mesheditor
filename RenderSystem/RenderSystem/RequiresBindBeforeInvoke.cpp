#include "RequiresBindBeforeInvoke.h"

namespace RenderSystem
{
  void RequiresBindBeforeInvoke::invoke(const std::function<void()>& action) const
  {
    bind();
    action();
    unbind();
  }
}  // namespace RenderSystem
