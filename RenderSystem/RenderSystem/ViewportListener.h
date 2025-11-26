#pragma once

namespace RenderSystem
{
  class Viewport;

  class ViewportListener
  {
  public:
    virtual ~ViewportListener() = default;

    virtual void onViewportChanged(Viewport* viewport) = 0;
  };
}

