#pragma once

#include <string>

#include "RequiresBindBeforeInvoke.h"

namespace RenderSystem
{
  class Texture : public RequiresBindBeforeInvoke
  {
   public:
    Texture() = default;
    Texture(int width, int height);
    virtual ~Texture();

    virtual void setDimensions(int width, int height) = 0;
    void passToFragmentShader(int textureLocation) const;

    unsigned int getId() const;
    int getWidth() const;
    int getHeight() const;

   private:
    void init();
    void bind() const override;
    void unbind() const override;

   protected:
    unsigned int mTexture = 0;
    int mWidth = 0;
    int mHeight = 0;
  };
}  // namespace RenderSystem
