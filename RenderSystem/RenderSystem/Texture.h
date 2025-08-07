#pragma once

#include <functional>

#include "RequiresBindBeforeInvoke.h"

namespace RenderSystem
{
  class Texture : public RequiresBindBeforeInvoke
  {
   public:
    Texture(int width, int height);
    virtual ~Texture();

    virtual void setDimensions(int width, int height) = 0;

    unsigned int getId() const;
    int getWidth() const;
    int getHeight() const;

   private:
    void init();
    void bind() override;
    void unbind() override;

   protected:
    unsigned int mTexture;
    int mWidth;
    int mHeight;
  };
}  // namespace RenderSystem
