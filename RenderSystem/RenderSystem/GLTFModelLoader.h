#pragma once

#include <fastgltf/core.hpp>
#include <filesystem>

#include "Object3D.h"

namespace RenderSystem
{
  class GLTFModelLoader
  {
    struct MaterialData
    {
      PBRMaterial material {};
      glm::vec2 uvScale {1.0f, 1.0f};
    };

   public:
    std::unique_ptr<Object3D> load(const std::filesystem::path& filePath);

   private:
    void parseAsset(Object3D* root);
    void parseNodes(size_t nodeIdx, Object3D* currentParentObject, Object3D* root);
    std::unique_ptr<Object3D> parseNode(const fastgltf::Node& node);
    std::unique_ptr<Mesh> parseMesh(const fastgltf::Node& node);
    std::vector<Vertex> parseVertices(size_t meshId);
    MaterialData parseMaterial(const fastgltf::Node& node);
    std::shared_ptr<ImageTexture> loadTexture(int imageIndex);
    std::shared_ptr<ImageTexture> loadTextureFromMemory(
      const fastgltf::sources::BufferView& bufferViewSource
    );
    Skeleton parseSkeleton(
      const fastgltf::Node& node, std::unordered_map<size_t, size_t>& nodeToJointMap
    );
    void parseJoints(
      const fastgltf::Skin& skin,
      const std::unordered_map<size_t, size_t>& nodeToJointMap,
      Skeleton& skeleton
    );
    std::vector<Animation> parseAnimations(
      const fastgltf::Node& node, const std::unordered_map<size_t, size_t>& nodeToJointMap
    );
    void parseChannels(
      const fastgltf::Animation& gltfAnimation,
      const std::unordered_map<size_t, size_t>& nodeToJointMap,
      Animation& animation
    ) const;

   private:
    fastgltf::Asset* mAsset = nullptr;
  };
}  // namespace RenderSystem
