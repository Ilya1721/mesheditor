#pragma once

#include <functional>

namespace RenderSystem
{
  class RequiresBindBeforeInvoke
  {
   public:
    virtual ~RequiresBindBeforeInvoke() = default;

    void invoke(const std::function<void()>& action) const;

   private:
    virtual void bind() const = 0;
    virtual void unbind() const = 0;

  protected:
    mutable int mResourceToRestore;
  };
}  // namespace RenderSystem
