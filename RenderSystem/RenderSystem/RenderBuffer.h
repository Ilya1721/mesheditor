#pragma once

namespace RenderSystem
{
  class RenderBuffer
  {
  public:
    virtual ~RenderBuffer() = default;
    virtual void bind() const = 0;
  };
}
