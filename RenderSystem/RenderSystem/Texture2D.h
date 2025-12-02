#pragma once

#include "Texture.h"

namespace RenderSystem
{
  class Texture2D : public Texture
  {
   public:
    Texture2D() = default;
    Texture2D(int width, int height);
    Texture2D(const Texture2D& other) = delete;
    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;

    int getWidth() const;
    int getHeight() const;

    virtual void setDimensions(int width, int height) = 0;
    void passToFragmentShader(int textureLocation, int textureSlot) const override;
    void swap(Texture2D& other) noexcept;

   protected:
    int mWidth = 0;
    int mHeight = 0;
  };
}  // namespace RenderSystem
