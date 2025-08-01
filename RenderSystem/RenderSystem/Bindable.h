#pragma once

#include <functional>

namespace RenderSystem
{
  class Bindable
  {
   public:
    virtual ~Bindable() = default;

    void invokeAction(const std::function<void()>& action);

   protected:
    virtual void bind() = 0;
    virtual void unbind() = 0;
  };
}  // namespace RenderSystem
