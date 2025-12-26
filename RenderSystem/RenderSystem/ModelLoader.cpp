#include "ModelLoader.h"

#include "GLTFModelLoader.h"
#include "Object3D.h"
#include "ObjModelLoader.h"
#include "STLModelLoader.h"
#include "Utility/StringHelper.h"

using namespace Utility;

namespace RenderSystem
{
  std::unique_ptr<Object3D> loadModelFromFile(const std::filesystem::path& filePath)
  {
    const auto& extension = filePath.extension().string();
    if (isEqual(extension, ".stl")) { return loadSTLModel(filePath); }
    else if (isEqual(extension, ".obj")) { return loadOBJModel(filePath); }
    else if (isEqual(extension, ".glb")) { return loadGLTFModel(filePath); }
    else { throw std::exception("Unsupported file format"); }
  }
}  // namespace RenderSystem