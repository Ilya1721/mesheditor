#include "ModelLoader.h"

#include "Constants.h"
#include "GLTFModelLoader.h"
#include "MeshCore/PointCloudUitls.h"
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
    else if (isEqual(extension, ".ply"))
    {
      PLYModelLoader loader;
      auto pointCloud = loader.loadPointCloud(filePath);
      auto mesh = pointCloudToMesh(pointCloud);
      return std::make_unique<Object3D>(std::move(mesh), RUBY_MATERIAL);
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
      return loader.loadPointCloud(filePath);
    }
    else
    {
      throw std::exception("Unsupported file format");
    }
  }
}  // namespace RenderSystem