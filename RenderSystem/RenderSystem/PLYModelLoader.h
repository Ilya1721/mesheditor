#pragma once

#include <filesystem>

#include "ModelLoaderUtils.h"
#include "Object3D.h"

namespace RenderSystem
{
  class PLYModelLoader : public ModelLoaderUtils
  {
   public:
    std::unique_ptr<Object3D> loadModel(const std::filesystem::path& filePath);
    std::vector<Vertex> loadVertices(const std::filesystem::path& filePath);

   private:
    std::vector<Vertex> loadBinary(const std::string& filePath);
    std::vector<Vertex> loadText(const std::string& filePath);
  };
}
