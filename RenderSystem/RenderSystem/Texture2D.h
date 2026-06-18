#pragma once

namespace RenderSystem
{
  class Texture2D
  {
   public:
    Texture2D(int width, int height);
    Texture2D(const Texture2D& other) = delete;
    ~Texture2D();

    unsigned int getId() const;
    int getWidth() const;
    int getHeight() const;

    void bind() const;

   protected:
    int mWidth;
    int mHeight;
    unsigned int mTexture;
  };
}  // namespace RenderSystem
