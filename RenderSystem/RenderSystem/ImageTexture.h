#pragma once

#include "Texture2D.h"

#include <string>

namespace RenderSystem
{
  class ImageTexture : public Texture2D
  {
   public:
    ImageTexture() = default;
    ImageTexture(int width, int height, int textureSlot);
    ImageTexture(const std::string& filePath, int TextureSlot);
    ImageTexture(const ImageTexture& other) = delete;
    ImageTexture(ImageTexture&& other) noexcept;
    ImageTexture& operator=(ImageTexture&& other) noexcept;

    void setDimensions(int width, int height) override;
    bool isEmpty() const;
    int getTextureSlot() const;

  private:
    unsigned char* mData = nullptr;
    int mColorChannels = 3;
    int mTextureSlot = 0;
  };
}  // namespace RenderSystem
