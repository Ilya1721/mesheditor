#include "GLTFModelLoader.h"

#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace RenderSystem
{
  class AccessorGetter
  {
   public:
    AccessorGetter(const fastgltf::Asset* asset, const fastgltf::Attribute* attributeEnd)
      : mAsset(asset), mAttributeEnd(attributeEnd)
    {
    }

   public:
    const fastgltf::Accessor* get(const fastgltf::Attribute* attribute)
    {
      if (attribute == mAttributeEnd)
      {
        return nullptr;
      }

      return &mAsset->accessors[attribute->accessorIndex];
    }

   private:
    const fastgltf::Asset* mAsset;
    const fastgltf::Attribute* mAttributeEnd;
  };

  std::vector<Vertex> parseVertices(
    const fastgltf::Asset& asset, const fastgltf::Mesh& mesh
  )
  {
    std::vector<Vertex> vertices;

    for (const auto& primitive : mesh.primitives)
    {
      auto posAttribute = primitive.findAttribute("POSITION");
      auto normalAttribute = primitive.findAttribute("NORMAL");
      auto textureAttribute = primitive.findAttribute("TEXCOORD_0");
      auto tangentAttribute = primitive.findAttribute("TANGENT");
      auto attributesEnd = primitive.attributes.end();

      if (posAttribute == attributesEnd || normalAttribute == attributesEnd)
      {
        throw std::runtime_error("Mesh primitive missing required attributes");
      }

      if (!primitive.indicesAccessor.has_value())
      {
        throw std::runtime_error("Non-indexed primitives not supported");
      }

      AccessorGetter accGetter(&asset, attributesEnd);
      auto posAcc = accGetter.get(posAttribute);
      auto normalAcc = accGetter.get(normalAttribute);
      auto textureAcc = accGetter.get(textureAttribute);
      auto tangentAcc = accGetter.get(tangentAttribute);
      const auto& indexAcc = asset.accessors[primitive.indicesAccessor.value()];
      Vertex vertex;

      for (size_t idx = 0; idx < indexAcc.count; ++idx)
      {
        auto vertexIdx = fastgltf::getAccessorElement<uint32_t>(asset, indexAcc, idx);
        auto posGltf =
          fastgltf::getAccessorElement<fastgltf::math::fvec3>(asset, *posAcc, vertexIdx);
        vertex.pos = glm::make_vec3(posGltf.data());
        auto normalGltf = fastgltf::getAccessorElement<fastgltf::math::fvec3>(
          asset, *normalAcc, vertexIdx
        );
        vertex.normal = glm::make_vec3(normalGltf.data());

        if (textureAcc)
        {
          auto textureGltf = fastgltf::getAccessorElement<fastgltf::math::fvec2>(
            asset, *textureAcc, vertexIdx
          );
          vertex.texture = glm::make_vec2(textureGltf.data());
        }

        if (tangentAcc)
        {
          auto tangentGltf = fastgltf::getAccessorElement<fastgltf::math::fvec4>(
            asset, *tangentAcc, vertexIdx
          );
          vertex.tangent = glm::make_vec4(tangentGltf.data());
        }

        vertices.push_back(vertex);
      }
    }

    return vertices;
  }

  std::unique_ptr<Mesh> parseMesh(
    const fastgltf::Asset& asset, const fastgltf::Node& node
  )
  {
    auto meshId = node.meshIndex.value();
    const auto& mesh = asset.meshes[meshId];
    auto vertices = parseVertices(asset, mesh);

    return std::make_unique<Mesh>(vertices);
  }

  struct MaterialData
  {
    PBRMaterial material {};
    Vector2D uvScale {1.0f, 1.0f};
  };

  struct MaterialImages
  {
    int baseColorImage = -1;
    int normalImage = -1;
    int metallicRoughnessImage = -1;
  };

  std::shared_ptr<ImageTexture> loadTextureFromMemory(
    const fastgltf::Asset& asset, const fastgltf::sources::BufferView& bufferViewSource
  )
  {
    const auto& bufferView = asset.bufferViews[bufferViewSource.bufferViewIndex];
    const auto& buffer = asset.buffers[bufferView.bufferIndex];
    std::shared_ptr<ImageTexture> imageTexture;

    std::visit(
      [&imageTexture, &bufferView](auto&& data)
      {
        using BufferDataType = std::decay_t<decltype(data)>;
        if constexpr (std::is_same_v<BufferDataType, fastgltf::sources::Array>)
        {
          auto convertedData = reinterpret_cast<const unsigned char*>(
            data.bytes.data() + bufferView.byteOffset
          );
          imageTexture = std::make_shared<ImageTexture>(convertedData, data.bytes.size());
        }
      },
      buffer.data
    );

    return imageTexture;
  }

  std::shared_ptr<ImageTexture> loadTexture(const fastgltf::Asset& asset, int imageIndex)
  {
    std::shared_ptr<ImageTexture> imageTexture;
    const fastgltf::Image& image = asset.images[imageIndex];
    std::visit(
      [&asset, &image, &imageTexture](auto&& data)
      {
        using ImageDataType = std::decay_t<decltype(data)>;
        if constexpr (std::is_same_v<ImageDataType, fastgltf::sources::BufferView>)
        {
          imageTexture = loadTextureFromMemory(asset, data);
        }
        else if constexpr (std::is_same_v<ImageDataType, fastgltf::sources::URI>)
        {
          imageTexture = std::make_shared<ImageTexture>(data.uri.c_str());
        }
      },
      image.data
    );

    return imageTexture;
  }

  MaterialData parseMaterial(const fastgltf::Asset& asset, const fastgltf::Node& node)
  {
    auto meshId = node.meshIndex.value();
    const auto& mesh = asset.meshes[meshId];
    const auto& primitive = mesh.primitives[0];

    MaterialData materialData;
    if (!primitive.materialIndex.has_value())
    {
      return materialData;
    }

    const auto& materialGltf = asset.materials[primitive.materialIndex.value()];
    MaterialImages materialImages;
    if (materialGltf.pbrData.baseColorTexture.has_value())
    {
      const auto& textureInfo = materialGltf.pbrData.baseColorTexture.value();
      const auto& texture = asset.textures[textureInfo.textureIndex];
      materialImages.baseColorImage = texture.imageIndex.value();
      if (textureInfo.transform)
      {
        materialData.uvScale = glm::make_vec2(textureInfo.transform->uvScale.data());
      }
      materialData.material.baseColorTexture =
        loadTexture(asset, materialImages.baseColorImage);
    }
    if (materialGltf.normalTexture.has_value())
    {
      const auto& textureInfo = materialGltf.normalTexture.value();
      const auto& texture = asset.textures[textureInfo.textureIndex];
      materialImages.normalImage = texture.imageIndex.value();
      materialData.material.normalMap = loadTexture(asset, materialImages.normalImage);
    }
    if (materialGltf.pbrData.metallicRoughnessTexture.has_value())
    {
      const auto& texInfo = materialGltf.pbrData.metallicRoughnessTexture.value();
      const auto& texture = asset.textures[texInfo.textureIndex];
      materialImages.metallicRoughnessImage = texture.imageIndex.value();
      materialData.material.metallicRougnessTexture =
        loadTexture(asset, materialImages.metallicRoughnessImage);
    }
    materialData.material.baseColor =
      glm::make_vec3(materialGltf.pbrData.baseColorFactor.data());
    materialData.material.metallic = materialGltf.pbrData.metallicFactor;
    materialData.material.rougness = materialGltf.pbrData.roughnessFactor;

    return materialData;
  }

  std::unique_ptr<Object3D> parseNode(
    const fastgltf::Asset& asset, const fastgltf::Node& node
  )
  {
    auto transformGltf = fastgltf::getTransformMatrix(node);
    auto transform = glm::make_mat4(transformGltf.data());
    auto mesh = parseMesh(asset, node);
    auto materialData = parseMaterial(asset, node);
    auto object3D = std::make_unique<Object3D>(std::move(mesh), materialData.material);
    object3D->updateTransform(transform);
    object3D->setUVScale(materialData.uvScale);

    return object3D;
  }

  void parseNodes(const fastgltf::Asset& asset, size_t nodeIdx, Object3D& parentObject)
  {
    const auto& node = asset.nodes[nodeIdx];
    auto childObject = parseNode(asset, node);
    for (const auto& childIdx : node.children)
    {
      parseNodes(asset, childIdx, *childObject);
    }
    parentObject.addChild(std::move(childObject));
  }

  void parseAsset(const fastgltf::Asset& asset, Object3D& root)
  {
    size_t defaultSceneIdx = asset.defaultScene.value();
    const auto& defaultScene = asset.scenes[defaultSceneIdx];
    for (const auto& nodeIdx : defaultScene.nodeIndices)
    {
      parseNodes(asset, nodeIdx, root);
    }
  }

  std::unique_ptr<Object3D> loadGLTFModel(const std::filesystem::path& filePath)
  {
    std::runtime_error runTimeErr("Failed to load glTF file: " + filePath.string());
    auto data = fastgltf::GltfDataBuffer::FromPath(filePath);
    if (data.error() != fastgltf::Error::None)
    {
      throw runTimeErr;
    }

    fastgltf::Parser parser;
    auto asset =
      parser.loadGltf(data.get(), filePath.parent_path(), fastgltf::Options::None);
    if (asset.error() != fastgltf::Error::None)
    {
      throw runTimeErr;
    }

    auto root = std::make_unique<Object3D>(std::make_unique<Mesh>(), PBRMaterial {});
    parseAsset(asset.get(), *root);

    return root;
  }
}  // namespace RenderSystem
