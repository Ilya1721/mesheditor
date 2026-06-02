#pragma once

#include <filesystem>

#include "ModelLoaderUtils.h"
#include "Object3D.h"

namespace RenderSystem
{
  class ObjMaterialLoader : public ModelLoaderUtils
  {
  public:
    BlinnPhongMaterial load(const std::filesystem::path& filePath);

  private:
    std::shared_ptr<ImageTexture> parseTexture(const std::filesystem::path& filePath);
    std::string parseTexturePath();
  };

  class OBJModelLoader : public ModelLoaderUtils
  {
   public:
    std::unique_ptr<Object3D> load(const std::filesystem::path& filePath);

   private:
    std::unique_ptr<Object3D> loadText(const std::filesystem::path& filePath);
    void parseFaceOBJ(
      std::vector<MeshCore::Vertex>& vertices,
      const std::vector<glm::vec3>& positions,
      const std::vector<glm::vec2>& textures,
      const std::vector<glm::vec3>& normals
    );
    std::string parseMaterialFileName();
  };
}  // namespace RenderSystem
