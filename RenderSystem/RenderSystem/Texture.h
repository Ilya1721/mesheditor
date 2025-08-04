#pragma once

#include <functional>

namespace RenderSystem
{
  class Texture
  {
   public:
    Texture(int width, int height);
    virtual ~Texture();

    virtual void setDimensions(int width, int height) = 0;

    void invokeEditAction(const std::function<void()>& action) const;

    unsigned int getId() const;
    int getWidth() const;
    int getHeight() const;

   private:
    void init();

   protected:
    unsigned int mTexture;
    int mWidth;
    int mHeight;
  };
}  // namespace RenderSystem
