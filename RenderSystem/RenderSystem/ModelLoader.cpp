#include "ModelLoader.h"

#include "Constants.h"
#include "GLTFModelLoader.h"
#include "MeshCore/PointCloudUitls.h"
#include "ObjModelLoader.h"
#include "Object3D.h"
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

      throw std::exception("Folder with unsupported files");
    }
  }

  std::unique_ptr<Object3D> loadModelFromFile(const fs::path& filePath)
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

  std::unique_ptr<Object3D> loadPLYModelFromDirectory(const fs::path& folderPath)
  {
    return {};
  }

  std::unique_ptr<Object3D> loadModelFromDirectory(const fs::path& folderPath)
  {
    auto folderFilesExtension = getFolderFilesExtension(folderPath);
    if (folderFilesExtension == ".ply")
    {
      return loadPLYModelFromDirectory(folderPath);
    }

    return {};
  }

  std::vector<Vertex> loadVerticesFromFile(const fs::path& filePath)
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

  std::vector<Vertex> loadPLYVerticesFromDirectory(const fs::path& folderPath)
  {
    return {};
  }

  std::vector<Vertex> loadVerticesFromDirectory(const fs::path& folderPath)
  {
    auto folderFilesExtension = getFolderFilesExtension(folderPath);
    if (folderFilesExtension == ".ply")
    {
      return loadPLYVerticesFromDirectory(folderPath);
    }

    return {};
  }
}

namespace RenderSystem
{
  std::unique_ptr<Object3D> loadModel(const std::filesystem::path& path)
  {
    if (fs::is_directory(path))
    {
      return loadModelFromDirectory(path);
    }

    return loadModelFromFile(path);
  }

  std::vector<Vertex> loadVertices(const std::filesystem::path& path)
  {
    if (fs::is_directory(path))
    {
      return loadVerticesFromDirectory(path);
    }

    return loadVerticesFromFile(path);
  }
}  // namespace RenderSystem