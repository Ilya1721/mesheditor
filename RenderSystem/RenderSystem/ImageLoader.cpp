#include "ImageLoader.h"

#include <stb_image/stb_image.h>

namespace RenderSystem
{
  ImageLoader::ImageLoader(const std::string& filePath)
    : mFilePath(filePath), mInputData(nullptr), mInputDataLength(0)
  {
  }

  ImageLoader::ImageLoader(const unsigned char* data, int dataLength)
    : mInputData(data), mInputDataLength(dataLength)
  {
  }

  ImageLoader::~ImageLoader()
  {
    stbi_image_free(mData);
  }

  const unsigned char* ImageLoader::getData()
  {
    return mData;
  }

  int ImageLoader::getWidth() const
  {
    return mWidth;
  }

  int ImageLoader::getHeight() const
  {
    return mHeight;
  }

  int ImageLoader::getColorChannels() const
  {
    return mColorChannels;
  }

  void ImageLoader::load()
  {
    if (!mFilePath.empty())
    {
      mData = stbi_load(mFilePath.c_str(), &mWidth, &mHeight, &mColorChannels, 0);
    }
    else
    {
      mData = stbi_load_from_memory(
        mInputData, mInputDataLength, &mWidth, &mHeight, &mColorChannels, 0
      );
    }
  }
}
