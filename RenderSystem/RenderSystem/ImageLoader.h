#pragma once

#include <functional>
#include <string>

namespace RenderSystem
{
  using TextureCreator =
    void(int width, int height, const unsigned char* decodedData, int colorChannels);

  void loadImage(
    const std::string& filePath, const std::function<TextureCreator>& textureCreator
  );
  void loadImage(
    const unsigned char* data,
    int dataLength,
    const std::function<TextureCreator>& textureCreator
  );
}
