#include "ImageLoader.h"

#include <stb_image/stb_image.h>

namespace RenderSystem
{
  void loadImage(
    const std::string& filePath, const std::function<TextureCreator>& textureCreator
  )
  {
    if (filePath.empty())
    {
      return;
    }
    int colorChannels, width, height;
    auto data = stbi_load(filePath.c_str(), &width, &height, &colorChannels, 0);
    textureCreator(width, height, data, colorChannels);
    stbi_image_free(data);
  }

  void loadImage(
    const unsigned char* data,
    int dataLength,
    const std::function<TextureCreator>& textureCreator
  )
  {
    if (data == nullptr || dataLength <= 0)
    {
      return;
    }
    int width, height, colorChannels;
    auto decodedData =
      stbi_load_from_memory(data, dataLength, &width, &height, &colorChannels, 0);
    textureCreator(width, height, decodedData, colorChannels);
    stbi_image_free(decodedData);
  }
}
