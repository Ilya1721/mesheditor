#pragma once

#include <memory>
#include <string>

#include "Texture2D.h"

namespace RenderSystem
{
  std::shared_ptr<Texture2D> createMotionVectorsTexture(int width, int height);
  std::shared_ptr<Texture2D> createColorTexture(int width, int height);
  std::shared_ptr<Texture2D> createDepthTexture(int width, int height);
  std::shared_ptr<Texture2D> createFlipbookTexture(const std::string& filePath);
  std::shared_ptr<Texture2D> createImageTexture(const std::string& filePath);
  std::shared_ptr<Texture2D> createImageTexture(const unsigned char* data, int dataSize);
}
