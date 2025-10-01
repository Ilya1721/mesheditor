#pragma once

#include "RequiresBindBeforeInvoke.h"

namespace RenderSystem
{
  class Texture : public RequiresBindBeforeInvoke
  {
   public:
    Texture();
    //Texture(int width, int height);
    Texture(const Texture& other) = delete;
    Texture(Texture&& other) noexcept;
    virtual ~Texture();
    Texture& operator=(Texture&& other) noexcept;

    //virtual void setDimensions(int width, int height) = 0;
    virtual void passToFragmentShader(int textureLocation) const = 0;

    unsigned int getId() const;
    //int getWidth() const;
    //int getHeight() const;

   private:
    //void init();
    void bind() const override;
    void unbind() const override;

   protected:
    unsigned int mTexture = 0;
    //int mWidth = 0;
    //int mHeight = 0;
  };
}  // namespace RenderSystem
