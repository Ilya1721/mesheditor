#include "GLTFModelLoader.h"

#include <fastgltf/tools.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "Animation.h"

namespace
{
  using namespace RenderSystem;

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

  struct MaterialImages
  {
    int baseColorImage = -1;
    int normalImage = -1;
    int metallicRoughnessImage = -1;
  };

  void parseJointRestPose(Joint& joint, const fastgltf::Node& node)
  {
    std::visit(
      [&joint, &node](auto&& transform)
      {
        auto& restPose = joint.restPose;
        using TransformType = std::decay_t<decltype(transform)>;
        if constexpr (std::is_same_v<TransformType, fastgltf::TRS>)
        {
          restPose.translation = glm::make_vec3(transform.translation.data());
          restPose.rotation = glm::quat(
            transform.rotation[3], transform.rotation[0], transform.rotation[1],
            transform.rotation[2]
          );
          restPose.scale = glm::make_vec3(transform.scale.data());
        }
        else if constexpr (std::is_same_v<TransformType, fastgltf::math::fmat4x4>)
        {
          auto glmMat4 = glm::make_mat4(transform.data());
          glm::vec3 skew {};
          glm::vec4 perspective {};
          glm::decompose(
            glmMat4, restPose.translation, restPose.rotation, restPose.scale, skew,
            perspective
          );
          restPose.rotation = glm::normalize(restPose.rotation);
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

  int findRootJointIdx(const std::vector<Joint>& joints)
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

  void calcJointGlobalTransform(
    Skeleton& skeleton, size_t jointIdx, const glm::mat4& parentGlobalTransform
  )
  {
    auto& joint = skeleton.joints[jointIdx];
    joint.globalTransform = parentGlobalTransform * joint.localTransform;
    joint.inverseGlobalTransform = glm::inverse(joint.globalTransform);
    for (const auto& childIdx : joint.childrenIndices)
    {
      calcJointGlobalTransform(skeleton, childIdx, joint.globalTransform);
    }
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

  Interpolation parseInterpolation(fastgltf::AnimationInterpolation interpolation)
  {
    switch (interpolation)
    {
      case fastgltf::AnimationInterpolation::Linear:
        return Interpolation::Linear;
      case fastgltf::AnimationInterpolation::Step:
        return Interpolation::Step;
    }

    return Interpolation::Unknown;
  }

  template <typename T>
  AnimationSampler<T> parseSampler(
    const fastgltf::Asset& asset,
    const fastgltf::AnimationSampler& gltfSampler,
    Animation& animation
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
      animation.duration = std::max(animation.duration, time);
      sampler.keyframes.emplace_back(time, value);
    }

    return sampler;
  }

  template <typename T>
  AnimationChannel<T> parseChannel(
    const fastgltf::Asset& asset,
    const fastgltf::Animation& gltfAnimation,
    const fastgltf::AnimationChannel& gltfChannel,
    const std::unordered_map<size_t, size_t>& nodeToJointMap,
    Animation& animation
  )
  {
    AnimationChannel<T> channel;
    channel.transformComponent = parseTransformComponent(gltfChannel.path);
    channel.jointIndex = nodeToJointMap.find(*gltfChannel.nodeIndex)->second;
    const auto& gltfSampler = gltfAnimation.samplers[gltfChannel.samplerIndex];
    channel.sampler = parseSampler<T>(asset, gltfSampler, animation);

    return channel;
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
}

namespace RenderSystem
{
  void GLTFModelLoader::parseChannels(
    const fastgltf::Animation& gltfAnimation,
    const std::unordered_map<size_t, size_t>& nodeToJointMap,
    Animation& animation
  ) const
  {
    for (const auto& gltfChannel : gltfAnimation.channels)
    {
      auto transformComponent = parseTransformComponent(gltfChannel.path);
      if (transformComponent == TransformComponent::Translation)
      {
        auto channel = parseChannel<glm::vec3>(
          *mAsset, gltfAnimation, gltfChannel, nodeToJointMap, animation
        );
        animation.translationChannels.push_back(channel);
      }
      else if (transformComponent == TransformComponent::Rotation)
      {
        auto channel = parseChannel<glm::quat>(
          *mAsset, gltfAnimation, gltfChannel, nodeToJointMap, animation
        );
        animation.rotationChannels.push_back(channel);
      }
      else if (transformComponent == TransformComponent::Scale)
      {
        auto channel = parseChannel<glm::vec3>(
          *mAsset, gltfAnimation, gltfChannel, nodeToJointMap, animation
        );
        animation.scaleChannels.push_back(channel);
      }
    }
  }

  std::vector<Animation> GLTFModelLoader::parseAnimations(
    const fastgltf::Node& node, const std::unordered_map<size_t, size_t>& nodeToJointMap
  )
  {
    std::vector<Animation> animations;
    for (const auto& gltfAnimation : mAsset->animations)
    {
      Animation animation;
      animation.name = gltfAnimation.name;
      parseChannels(gltfAnimation, nodeToJointMap, animation);
      animations.push_back(animation);
    }

    return animations;
  }

  void GLTFModelLoader::parseJoints(
    const fastgltf::Skin& skin,
    const std::unordered_map<size_t, size_t>& nodeToJointMap,
    Skeleton& skeleton
  )
  {
    skeleton.joints.resize(skin.joints.size());
    for (size_t jointIdx = 0; jointIdx < skin.joints.size(); ++jointIdx)
    {
      auto& joint = skeleton.joints[jointIdx];
      auto nodeIdx = skin.joints[jointIdx];
      const auto& node = mAsset->nodes[nodeIdx];
      joint.name = node.name;
      parseJointRestPose(joint, node);
      parseJointParent(node, jointIdx, nodeToJointMap, skeleton);
      joint.localTransform = calcJointLocalTransform(joint.restPose);
      skeleton.nameIndexMap.insert({joint.name, skeleton.joints.size() - 1});
    }
  }

  Skeleton GLTFModelLoader::parseSkeleton(
    const fastgltf::Node& node, std::unordered_map<size_t, size_t>& nodeToJointMap
  )
  {
    if (!node.skinIndex.has_value())
    {
      return {};
    }

    auto skinIndex = *node.skinIndex;
    const auto& skin = mAsset->skins[skinIndex];
    nodeToJointMap = mapNodeToJoint(skin);

    Skeleton skeleton;
    parseJoints(skin, nodeToJointMap, skeleton);
    skeleton.rootJointIndex = findRootJointIdx(skeleton.joints);
    calcJointGlobalTransform(skeleton, skeleton.rootJointIndex, glm::mat4(1.0f));

    return skeleton;
  }

  std::shared_ptr<ImageTexture> GLTFModelLoader::loadTextureFromMemory(
    const fastgltf::sources::BufferView& bufferViewSource
  )
  {
    const auto& bufferView = mAsset->bufferViews[bufferViewSource.bufferViewIndex];
    const auto& buffer = mAsset->buffers[bufferView.bufferIndex];
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

  std::shared_ptr<ImageTexture> GLTFModelLoader::loadTexture(int imageIndex)
  {
    std::shared_ptr<ImageTexture> imageTexture;
    const fastgltf::Image& image = mAsset->images[imageIndex];
    std::visit(
      [this, &image, &imageTexture](auto&& data)
      {
        using ImageDataType = std::decay_t<decltype(data)>;
        if constexpr (std::is_same_v<ImageDataType, fastgltf::sources::BufferView>)
        {
          imageTexture = loadTextureFromMemory(data);
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

  MaterialData GLTFModelLoader::parseMaterial(const fastgltf::Node& node)
  {
    auto meshId = node.meshIndex.value();
    const auto& mesh = mAsset->meshes[meshId];
    const auto& primitive = mesh.primitives[0];

    MaterialData materialData;
    if (!primitive.materialIndex.has_value())
    {
      return materialData;
    }

    const auto& materialGltf = mAsset->materials[primitive.materialIndex.value()];
    MaterialImages materialImages;
    if (materialGltf.pbrData.baseColorTexture.has_value())
    {
      const auto& textureInfo = materialGltf.pbrData.baseColorTexture.value();
      const auto& texture = mAsset->textures[textureInfo.textureIndex];
      materialImages.baseColorImage = texture.imageIndex.value();
      if (textureInfo.transform)
      {
        materialData.uvScale = glm::make_vec2(textureInfo.transform->uvScale.data());
      }
      materialData.material.baseColorTexture = loadTexture(materialImages.baseColorImage);
    }
    if (materialGltf.normalTexture.has_value())
    {
      const auto& textureInfo = materialGltf.normalTexture.value();
      const auto& texture = mAsset->textures[textureInfo.textureIndex];
      materialImages.normalImage = texture.imageIndex.value();
      materialData.material.normalMap = loadTexture(materialImages.normalImage);
    }
    if (materialGltf.pbrData.metallicRoughnessTexture.has_value())
    {
      const auto& texInfo = materialGltf.pbrData.metallicRoughnessTexture.value();
      const auto& texture = mAsset->textures[texInfo.textureIndex];
      materialImages.metallicRoughnessImage = texture.imageIndex.value();
      materialData.material.metallicRougnessTexture =
        loadTexture(materialImages.metallicRoughnessImage);
    }
    materialData.material.baseColor =
      glm::make_vec3(materialGltf.pbrData.baseColorFactor.data());
    materialData.material.metallic = materialGltf.pbrData.metallicFactor;
    materialData.material.rougness = materialGltf.pbrData.roughnessFactor;

    return materialData;
  }

  std::vector<Vertex> GLTFModelLoader::parseVertices(size_t meshId)
  {
    std::vector<Vertex> vertices;
    const auto& mesh = mAsset->meshes[meshId];

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

      AccessorGetter accGetter(mAsset, attributesEnd);
      auto posAcc = accGetter.get(posAttribute);
      auto normalAcc = accGetter.get(normalAttribute);
      auto textureAcc = accGetter.get(textureAttribute);
      auto tangentAcc = accGetter.get(tangentAttribute);
      auto jointIndicesAcc = accGetter.get(jointIndicesAttribute);
      auto weightsAcc = accGetter.get(weightsAttribute);
      const auto& indexAcc = mAsset->accessors[primitive.indicesAccessor.value()];
      Vertex vertex;

      for (size_t idx = 0; idx < indexAcc.count; ++idx)
      {
        auto vertexIdx = fastgltf::getAccessorElement<uint32_t>(*mAsset, indexAcc, idx);
        auto posGltf = fastgltf::getAccessorElement<fastgltf::math::fvec3>(
          *mAsset, *posAcc, vertexIdx
        );
        vertex.pos = glm::make_vec3(posGltf.data());
        auto normalGltf = fastgltf::getAccessorElement<fastgltf::math::fvec3>(
          *mAsset, *normalAcc, vertexIdx
        );
        vertex.normal = glm::make_vec3(normalGltf.data());

        if (textureAcc)
        {
          auto textureGltf = fastgltf::getAccessorElement<fastgltf::math::fvec2>(
            *mAsset, *textureAcc, vertexIdx
          );
          vertex.texture = glm::make_vec2(textureGltf.data());
        }

        if (tangentAcc)
        {
          auto tangentGltf = fastgltf::getAccessorElement<fastgltf::math::fvec4>(
            *mAsset, *tangentAcc, vertexIdx
          );
          vertex.tangent = glm::make_vec4(tangentGltf.data());
        }

        if (jointIndicesAcc)
        {
          auto jointIndicesGltf = fastgltf::getAccessorElement<fastgltf::math::uvec4>(
            *mAsset, *jointIndicesAcc, vertexIdx
          );
          vertex.jointIndices = glm::make_vec4(jointIndicesGltf.data());
        }

        if (weightsAcc)
        {
          auto weightsGltf = fastgltf::getAccessorElement<fastgltf::math::fvec4>(
            *mAsset, *weightsAcc, vertexIdx
          );
          vertex.weights = parseWeights(*weightsAcc, weightsGltf);
        }

        vertices.push_back(vertex);
      }
    }

    return vertices;
  }

  std::unique_ptr<Mesh> GLTFModelLoader::parseMesh(const fastgltf::Node& node)
  {
    auto meshId = node.meshIndex.value();
    auto vertices = parseVertices(meshId);
    return std::make_unique<Mesh>(vertices);
  }

  std::unique_ptr<Object3D> GLTFModelLoader::parseNode(const fastgltf::Node& node)
  {
    if (!node.meshIndex.has_value())
    {
      return nullptr;
    }

    auto transformGltf = fastgltf::getTransformMatrix(node);
    auto transform = glm::make_mat4(transformGltf.data());
    auto mesh = parseMesh(node);
    auto materialData = parseMaterial(node);
    std::unordered_map<size_t, size_t> nodeToJointMap;
    auto skeleton = parseSkeleton(node, nodeToJointMap);
    auto animations = parseAnimations(node, nodeToJointMap);
    auto object3D = std::make_unique<Object3D>(
      std::move(mesh), materialData.material, std::move(skeleton), std::move(animations)
    );
    object3D->updateTransform(transform);
    object3D->setUVScale(materialData.uvScale);

    return object3D;
  }

  void GLTFModelLoader::parseNodes(
    size_t nodeIdx, Object3D* currentParentObject, Object3D* root
  )
  {
    const auto& node = mAsset->nodes[nodeIdx];
    auto object3D = parseNode(node);
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
      parseNodes(childIdx, parentObject, root);
    }
  }

  void GLTFModelLoader::parseAsset(Object3D* root)
  {
    size_t defaultSceneIdx = mAsset->defaultScene.value();
    const auto& defaultScene = mAsset->scenes[defaultSceneIdx];
    for (const auto& nodeIdx : defaultScene.nodeIndices)
    {
      parseNodes(nodeIdx, root, root);
    }
  }

  std::unique_ptr<Object3D> GLTFModelLoader::load(const std::filesystem::path& filePath)
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

    mAsset = &asset.get();
    auto root = std::make_unique<Object3D>(std::make_unique<Mesh>(), PBRMaterial {});
    parseAsset(root.get());

    return root;
  }
}
