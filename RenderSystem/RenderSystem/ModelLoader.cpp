#include "ModelLoader.h"

#include "Constants.h"
#include "GLTFModelLoader.h"
#include "MeshCore/PointCloudUtils.h"
#include "ObjModelLoader.h"
#include "PLYModelLoader.h"
#include "STLModelLoader.h"
#include "Utility/StringHelper.h"

namespace fs = std::filesystem;
using namespace Utility;

namespace
{
  using namespace RenderSystem;

  std::string getFolderFilesExtension(const fs::path& folderPath)
  {
    for (const auto& entry : fs::directory_iterator(folderPath))
    {
      if (!entry.is_regular_file())
      {
        continue;
      }

      if (entry.path().extension().string() == ".ply")
      {
        return ".ply";
      }
    }

    throw std::exception("Folder with unsupported files");
  }

  std::unique_ptr<Object3D> loadModelFromFile(
    const fs::path& filePath, const ModelLoaderConfig& config
  )
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
      return loader.loadPointCloud(filePath, config);
    }
    else
    {
      throw std::exception("Unsupported file format");
    }
  }

  std::unique_ptr<Object3D> loadModelFromDirectory(
    const fs::path& folderPath, const ModelLoaderConfig& config
  )
  {
    auto folderFilesExtension = getFolderFilesExtension(folderPath);
    if (folderFilesExtension == ".ply")
    {
      PLYModelLoader loader;
      return loader.loadMultiplePointClouds(folderPath, config);
    }

    return {};
  }
}

namespace RenderSystem
{
  std::unique_ptr<Object3D> loadModel(
    const std::filesystem::path& path, const ModelLoaderConfig& config
  )
  {
    if (fs::is_directory(path))
    {
      return loadModelFromDirectory(path, config);
    }

    return loadModelFromFile(path, config);
  }
}  // namespace RenderSystem