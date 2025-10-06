#pragma once

#include "RenderData.h"
#include "RequiresBindBeforeInvoke.h"

namespace RenderSystem
{
  class RenderBuffer : public RequiresBindBeforeInvoke
  {
   public:
    RenderBuffer();
    RenderBuffer(RenderBuffer&&) = delete;
    ~RenderBuffer();

    void loadRenderData(const RenderData& renderData);

   private:
    void bind() const override;
    void unbind() const override;
    void init();

   private:
    unsigned int mVBO;
    unsigned int mVAO;
    mutable int mSavedVBO;
    mutable int mSavedVAO;
  };
}  // namespace RenderSystem
