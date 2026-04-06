#include "GLTFModelLoader.h"

#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "Animation.h"

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

  std::vector<glm::mat4> parseInverseBindMatrices(
    const fastgltf::Asset& asset, const fastgltf::Skin& skin
  )
  {
    if (!skin.inverseBindMatrices.has_value())
    {
      return {};
    }

    std::vector<glm::mat4> inverseBindMatrices;
    const auto& accessor = asset.accessors[*skin.inverseBindMatrices];
    fastgltf::iterateAccessorWithIndex<fastgltf::math::fmat4x4>(
      asset,
      accessor,
      [&inverseBindMatrices](const fastgltf::math::fmat4x4& matrix, size_t index)
      { inverseBindMatrices.push_back(glm::make_mat4(matrix.data())); }
    );

    return inverseBindMatrices;
  }

  void parseJointTransform(Joint& joint, const fastgltf::Node& node)
  {
    std::visit(
      [&joint, &node](auto&& transform)
      {
        using TransformType = std::decay_t<decltype(transform)>;
        if constexpr (std::is_same_v<TransformType, fastgltf::TRS>)
        {
          joint.translation = glm::make_vec3(transform.translation.data());
          joint.rotation = glm::quat(
            transform.rotation[3], transform.rotation[0], transform.rotation[1],
            transform.rotation[2]
          );
          joint.scale = glm::make_vec3(transform.scale.data());
        }
        else if constexpr (std::is_same_v<TransformType, fastgltf::math::fmat4x4>)
        {
          auto glmMat4 = glm::make_mat4(transform.data());
          glm::vec3 skew {};
          glm::vec4 perspective {};
          glm::decompose(
            glmMat4, joint.translation, joint.rotation, joint.scale, skew, perspective
          );
          joint.rotation = glm::normalize(joint.rotation);
        }
      },
      node.transform
    );
  }

  void parseJointParent(
    const fastgltf::Node& node,
    size_t jointIdx,
    const std::unordered_map<size_t, size_t>& nodeToJointMap,
    Skeleton& skeleton
  )
  {
    for (auto child : node.children)
    {
      auto childNodeIt = nodeToJointMap.find(child);
      if (childNodeIt != nodeToJointMap.end())
      {
        auto childJointIdx = childNodeIt->second;
        skeleton.joints[jointIdx].childrenIndices.push_back(childJointIdx);
        skeleton.joints[childJointIdx].parentIndex = jointIdx;
      }
    }
  }

  void parseJoints(
    const fastgltf::Asset& asset,
    const fastgltf::Skin& skin,
    const fastgltf::Node& node,
    const std::unordered_map<size_t, size_t>& nodeToJointMap,
    Skeleton& skeleton
  )
  {
    skeleton.joints.resize(skin.joints.size());
    auto inverseBindMatrices = parseInverseBindMatrices(asset, skin);
    for (size_t jointIdx = 0; jointIdx < skin.joints.size(); ++jointIdx)
    {
      auto& joint = skeleton.joints[jointIdx];
      auto nodeIdx = skin.joints[jointIdx];
      const auto& node = asset.nodes[nodeIdx];
      joint.name = node.name;
      parseJointTransform(joint, node);
      if (!inverseBindMatrices.empty())
      {
        joint.inverseBindMatrix = inverseBindMatrices[jointIdx];
      }
      else
      {
        joint.inverseBindMatrix = glm::mat4(1.0f);
      }
      skeleton.nameIndexMap.insert({joint.name, skeleton.joints.size() - 1});
      parseJointParent(node, jointIdx, nodeToJointMap, skeleton);
    }
  }

  size_t findRootJointIdx(const std::vector<Joint>& joints)
  {
    for (size_t jointIdx = 0; jointIdx < joints.size(); ++jointIdx)
    {
      if (joints[jointIdx].parentIndex == -1)
      {
        return jointIdx;
      }
    }

    return -1;
  }

  std::unordered_map<size_t, size_t> mapNodeToJoint(const fastgltf::Skin& skin)
  {
    std::unordered_map<size_t, size_t> nodeToJoint;
    for (size_t jointIdx = 0; jointIdx < skin.joints.size(); ++jointIdx)
    {
      nodeToJoint[skin.joints[jointIdx]] = jointIdx;
    }

    return nodeToJoint;
  }

  Skeleton parseSkeleton(
    const fastgltf::Asset& asset,
    const fastgltf::Node& node,
    std::unordered_map<size_t, size_t>& nodeToJointMap
  )
  {
    if (!node.skinIndex.has_value())
    {
      return {};
    }

    auto skinIndex = *node.skinIndex;
    const auto& skin = asset.skins[skinIndex];
    nodeToJointMap = mapNodeToJoint(skin);

    Skeleton skeleton;
    parseJoints(asset, skin, node, nodeToJointMap, skeleton);
    skeleton.rootJointIndex = findRootJointIdx(skeleton.joints);

    return skeleton;
  }

  TransformComponent parseTransformComponent(fastgltf::AnimationPath animationPath)
  {
    switch (animationPath)
    {
      case fastgltf::AnimationPath::Translation:
        return TransformComponent::Translation;
      case fastgltf::AnimationPath::Rotation:
        return TransformComponent::Rotation;
      case fastgltf::AnimationPath::Scale:
        return TransformComponent::Scale;
    }

    return TransformComponent::Unknown;
  }

  std::vector<AnimationChannel> parseChannels(
    const fastgltf::Animation& animation,
    const std::unordered_map<size_t, size_t>& nodeToJointMap,
    std::vector<TransformComponent>& samplerTypes
  )
  {
    std::vector<AnimationChannel> channels;
    for (const auto& gltfChannel : animation.channels)
    {
      AnimationChannel channel {};
      channel.samplerIndex = gltfChannel.samplerIndex;
      auto nodeToJointIt = nodeToJointMap.find(*gltfChannel.nodeIndex);
      if (nodeToJointIt != nodeToJointMap.end())
      {
        channel.jointIndex = nodeToJointIt->second;
      }
      auto transformComponent = parseTransformComponent(gltfChannel.path);
      samplerTypes[channel.samplerIndex] = transformComponent;
      channel.transformComponent = transformComponent;
      channels.push_back(channel);
    }

    return channels;
  }

  Interpolation parseInterpolation(fastgltf::AnimationInterpolation interpolation)
  {
    switch (interpolation)
    {
      case fastgltf::AnimationInterpolation::Linear:
        return Interpolation::Linear;
      case fastgltf::AnimationInterpolation::Step:
        return Interpolation::Step;
      case fastgltf::AnimationInterpolation::CubicSpline:
        return Interpolation::CubicSpline;
    }

    return Interpolation::Unknown;
  }

  template <typename T>
  T parseAnimationAccessorValue(
    const fastgltf::Asset& asset, const fastgltf::Accessor& accessor, size_t keyframeIdx
  )
  {
    if constexpr (std::is_same_v<T, glm::vec3>)
    {
      auto vec3Gltf =
        fastgltf::getAccessorElement<fastgltf::math::fvec3>(asset, accessor, keyframeIdx);
      return glm::make_vec3(vec3Gltf.data());
    }
    else if constexpr (std::is_same_v<T, glm::quat>)
    {
      auto vec4Gltf =
        fastgltf::getAccessorElement<fastgltf::math::fvec4>(asset, accessor, keyframeIdx);
      auto vec4 = glm::make_vec4(vec4Gltf.data());
      glm::quat value(vec4.w, vec4.x, vec4.y, vec4.z);
      return glm::normalize(value);
    }

    return T();
  }

  template <typename T>
  AnimationSampler<T> parseSampler(
    const fastgltf::Asset& asset, const fastgltf::AnimationSampler& gltfSampler
  )
  {
    AnimationSampler<T> sampler;
    sampler.interpolation = parseInterpolation(gltfSampler.interpolation);
    const auto& inputAcc = asset.accessors[gltfSampler.inputAccessor];
    const auto& outputAcc = asset.accessors[gltfSampler.outputAccessor];

    for (size_t keyframeIdx = 0; keyframeIdx < inputAcc.count; ++keyframeIdx)
    {
      auto time = fastgltf::getAccessorElement<float>(asset, inputAcc, keyframeIdx);
      auto value = parseAnimationAccessorValue<T>(asset, outputAcc, keyframeIdx);
      sampler.keyframes.emplace_back(time, value);
    }

    return sampler;
  }

  void parseSamplers(
    const fastgltf::Asset& asset,
    const fastgltf::Animation& gltfAnimation,
    const std::vector<TransformComponent>& samplerTypes,
    Animation& animation
  )
  {
    for (size_t samplerIdx = 0; samplerIdx < gltfAnimation.samplers.size(); ++samplerIdx)
    {
      const auto& samplerType = samplerTypes[samplerIdx];
      const auto& gltfSampler = gltfAnimation.samplers[samplerIdx];
      if (samplerType == TransformComponent::Translation)
      {
        auto sampler = parseSampler<glm::vec3>(asset, gltfSampler);
        animation.translationSamplers.push_back(sampler);
      }
      else if (samplerType == TransformComponent::Rotation)
      {
        auto sampler = parseSampler<glm::quat>(asset, gltfSampler);
        animation.rotationSamplers.push_back(sampler);
      }
      else if (samplerType == TransformComponent::Scale)
      {
        auto sampler = parseSampler<glm::vec3>(asset, gltfSampler);
        animation.scaleSamplers.push_back(sampler);
      }
    }
  }

  std::vector<Animation> parseAnimations(
    const fastgltf::Asset& asset,
    const fastgltf::Node& node,
    const std::unordered_map<size_t, size_t>& nodeToJointMap
  )
  {
    std::vector<Animation> animations;
    for (const auto& gltfAnimation : asset.animations)
    {
      Animation animation;
      std::vector<TransformComponent> samplerTypes(
        gltfAnimation.samplers.size(), TransformComponent::Unknown
      );
      animation.channels = parseChannels(gltfAnimation, nodeToJointMap, samplerTypes);
      parseSamplers(asset, gltfAnimation, samplerTypes, animation);
      animations.push_back(animation);
    }

    return animations;
  }

  glm::vec4 parseWeights(
    const fastgltf::Accessor& weightsAcc, const fastgltf::math::fvec4& weightsGltf
  )
  {
    auto weights = glm::make_vec4(weightsGltf.data());
    if (weightsAcc.componentType == fastgltf::ComponentType::UnsignedByte)
    {
      weights /= 255.0f;
    }
    else if (weightsAcc.componentType == fastgltf::ComponentType::UnsignedShort)
    {
      weights /= 65535.0f;
    }

    auto sum = glm::compAdd(weights);
    if (sum > 0.0f)
    {
      weights /= sum;
    }

    return weights;
  }

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
      auto jointIndicesAttribute = primitive.findAttribute("JOINTS_0");
      auto weightsAttribute = primitive.findAttribute("WEIGHTS_0");
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
      auto jointIndicesAcc = accGetter.get(jointIndicesAttribute);
      auto weightsAcc = accGetter.get(weightsAttribute);
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

        if (jointIndicesAcc)
        {
          auto jointIndicesGltf = fastgltf::getAccessorElement<fastgltf::math::uvec4>(
            asset, *jointIndicesAcc, vertexIdx
          );
          vertex.jointIndices = glm::make_vec4(jointIndicesGltf.data());
        }

        if (weightsAcc)
        {
          auto weightsGltf = fastgltf::getAccessorElement<fastgltf::math::fvec4>(
            asset, *weightsAcc, vertexIdx
          );
          vertex.weights = parseWeights(*weightsAcc, weightsGltf);
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
    if (!node.meshIndex.has_value())
    {
      return nullptr;
    }

    auto transformGltf = fastgltf::getTransformMatrix(node);
    auto transform = glm::make_mat4(transformGltf.data());
    auto mesh = parseMesh(asset, node);
    auto materialData = parseMaterial(asset, node);
    std::unordered_map<size_t, size_t> nodeToJointMap;
    auto skeleton = parseSkeleton(asset, node, nodeToJointMap);
    auto animations = parseAnimations(asset, node, nodeToJointMap);
    auto object3D = std::make_unique<Object3D>(std::move(mesh), materialData.material);
    object3D->updateTransform(transform);
    object3D->setUVScale(materialData.uvScale);

    return object3D;
  }

  void parseNodes(
    const fastgltf::Asset& asset,
    size_t nodeIdx,
    Object3D* currentParentObject,
    Object3D* root
  )
  {
    const auto& node = asset.nodes[nodeIdx];
    auto object3D = parseNode(asset, node);
    Object3D* parentObject = nullptr;
    if (object3D)
    {
      parentObject = object3D.get();
      currentParentObject->addChild(std::move(object3D));
    }
    else
    {
      parentObject = root;
    }
    for (const auto& childIdx : node.children)
    {
      parseNodes(asset, childIdx, parentObject, root);
    }
  }

  void parseAsset(const fastgltf::Asset& asset, Object3D* root)
  {
    size_t defaultSceneIdx = asset.defaultScene.value();
    const auto& defaultScene = asset.scenes[defaultSceneIdx];
    for (const auto& nodeIdx : defaultScene.nodeIndices)
    {
      parseNodes(asset, nodeIdx, root, root);
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
    parseAsset(asset.get(), root.get());

    return root;
  }
}  // namespace RenderSystem
