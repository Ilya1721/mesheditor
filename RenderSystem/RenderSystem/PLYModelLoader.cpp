#include "PLYModelLoader.h"

namespace RenderSystem
{
  std::unique_ptr<Object3D> PLYModelLoader::loadModel(const std::filesystem::path& filePath)
  {
    return {};
  }

  std::vector<Vertex> PLYModelLoader::loadVertices(const std::filesystem::path& filePath)
  {
    return {};
  }

  std::vector<Vertex> PLYModelLoader::loadBinary(const std::string& filePath)
  {
    return {};
  }

  std::vector<Vertex> PLYModelLoader::loadText(const std::string& filePath)
  {
    return {};
  }
}
