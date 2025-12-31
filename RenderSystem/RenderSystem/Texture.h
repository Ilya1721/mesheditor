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

    unsigned int getId() const;

    virtual void passToFragmentShader(int textureLocation, int textureSlot) const = 0;
    virtual int getAttachmentId() const = 0;

   private:
    void bind() const override;
    void unbind() const override;

   protected:
    unsigned int mTexture = 0;
  };

  int getColorFormat(int colorChannels);
}  // namespace RenderSystem
