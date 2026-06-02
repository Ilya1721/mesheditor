#pragma once

#include <filesystem>

#include "Object3D.h"

#include "ModelLoaderUtils.h"

namespace RenderSystem
{
  class STLModelLoader : public ModelLoaderUtils
  {
   public:
    std::unique_ptr<Object3D> loadModel(const std::filesystem::path& filePath);

   private:
    bool isBinary() const;
    std::unique_ptr<Object3D> loadBinary();
    std::unique_ptr<Object3D> loadText();
  };
}  // namespace RenderSystem
