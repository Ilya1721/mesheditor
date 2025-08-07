#pragma once

#include <functional>

namespace RenderSystem
{
  class RequiresBindBeforeInvoke
  {
   public:
    virtual ~RequiresBindBeforeInvoke() = default;

    void invoke(const std::function<void()>& action);

   private:
    virtual void bind() = 0;
    virtual void unbind() = 0;

  protected:
    int mResourceToRestore;
  };
}  // namespace RenderSystem
