#pragma once

#include <string>

#include "Texture2D.h"

namespace RenderSystem
{
  class ImageTexture : public Texture2D
  {
   public:
    ImageTexture(int width, int height);
    ImageTexture(const std::string& filePath);
    ImageTexture(const ImageTexture& other) = delete;
    ImageTexture(ImageTexture&& other) noexcept;
    ImageTexture& operator=(ImageTexture&& other) noexcept;

    int getAttachmentId() const override;

    void create(int width, int height) override;

   private:
    unsigned char* mData = nullptr;
    int mColorChannels = 4;
  };
}  // namespace RenderSystem
