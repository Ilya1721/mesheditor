#include "ModelLoader.h"

#include "GLTFModelLoader.h"
#include "ObjModelLoader.h"
#include "Object3D.h"
#include "PLYModelLoader.h"
#include "STLModelLoader.h"
#include "Utility/StringHelper.h"

using namespace Utility;

namespace RenderSystem
{
  std::unique_ptr<Object3D> loadModelFromFile(const std::filesystem::path& filePath)
  {
    const auto& extension = filePath.extension().string();
    if (isEqual(extension, ".stl"))
    {
      STLModelLoader loader;
      return loader.loadModel(filePath);
    }
    else if (isEqual(extension, ".obj"))
    {
      OBJModelLoader loader;
      return loader.load(filePath);
    }
    else if (isEqual(extension, ".glb"))
    {
      GLTFModelLoader loader;
      return loader.load(filePath);
    }
    else if (isEqual(extension, ".glb"))
    {
      PLYModelLoader loader;
      return loader.loadModel(filePath);
    }
    else
    {
      throw std::exception("Unsupported file format");
    }
  }

  std::vector<Vertex> loadVerticesFromFile(const std::filesystem::path& filePath)
  {
    const auto& extension = filePath.extension().string();
    if (isEqual(extension, ".ply"))
    {
      PLYModelLoader loader;
      return loader.loadVertices(filePath);
    }
    else
    {
      throw std::exception("Unsupported file format");
    }
  }
}  // namespace RenderSystem