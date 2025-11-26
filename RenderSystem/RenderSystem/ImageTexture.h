#pragma once

#include <string>

#include "Texture2D.h"

namespace RenderSystem
{
  class ImageTexture : public Texture2D
  {
   public:
    ImageTexture() = default;
    ImageTexture(int width, int height);
    ImageTexture(const std::string& filePath);
    ImageTexture(const ImageTexture& other) = delete;
    ImageTexture(ImageTexture&& other) noexcept;
    ImageTexture& operator=(ImageTexture&& other) noexcept;

    bool isEmpty() const;
    int getAttachmentId() const override;

    void setDimensions(int width, int height) override;

   private:
    unsigned char* mData = nullptr;
    int mColorChannels = 3;
  };
}  // namespace RenderSystem
