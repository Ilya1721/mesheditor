#pragma once

#include <functional>
#include <string>

namespace RenderSystem
{
  class ImageLoader
  {
   public:
    ImageLoader(const std::string& filePath);
    ImageLoader(const unsigned char* data, int dataLength);
    ~ImageLoader();

    const unsigned char* getData();
    int getWidth() const;
    int getHeight() const;
    int getColorChannels() const;

    void load();

   private:
    unsigned char* mData;
    int mWidth;
    int mHeight;
    int mColorChannels;

    const unsigned char* mInputData;
    int mInputDataLength;
    std::string mFilePath;
  };
}
