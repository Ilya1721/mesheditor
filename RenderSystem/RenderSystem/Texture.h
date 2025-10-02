#pragma once

#include "RequiresBindBeforeInvoke.h"

namespace RenderSystem
{
  class Texture : public RequiresBindBeforeInvoke
  {
   public:
    Texture();
    Texture(const Texture& other) = delete;
    Texture(Texture&& other) noexcept;
    virtual ~Texture();
    Texture& operator=(Texture&& other) noexcept;

    virtual void passToFragmentShader(int textureLocation) const = 0;

    unsigned int getId() const;

   private:
    void bind() const override;
    void unbind() const override;

   protected:
    unsigned int mTexture = 0;
  };
}  // namespace RenderSystem
