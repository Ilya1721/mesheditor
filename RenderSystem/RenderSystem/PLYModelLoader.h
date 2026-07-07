#pragma once

#include <filesystem>
#include <vector>

#include "ModelLoaderUtils.h"
#include "Object3D.h"

namespace RenderSystem
{
  using namespace MeshCore;

  class PLYModelLoader : public ModelLoaderUtils
  {
    struct PropertyType
    {
      std::string type;
      std::string property;
    };

    struct Header
    {
      bool isBinary {};
      size_t vertexCount {};
      std::vector<PropertyType> propertyTypes;
    };

    using ModelTransform = std::pair<std::string, glm::mat4>;

   public:
    PLYModelLoader(const ModelLoaderConfig& config);

    std::unique_ptr<Object3D> loadPointCloud(const std::filesystem::path& filePath);
    std::unique_ptr<Object3D> loadPointClouds(const std::filesystem::path& folderPath);

   private:
    std::vector<Vertex> loadVertices(
      const std::filesystem::path& filePath, const glm::mat4& transform = glm::mat4(1.0f)
    );
    std::vector<Vertex> parseVertices(const glm::mat4& transform = glm::mat4(1.0f));
    std::vector<Vertex> parseMultipleFiles(const std::filesystem::path& folderPath);
    std::vector<Vertex> parseFilesFromConf(const std::filesystem::path& confPath);
    std::vector<ModelTransform> parseConfFile(const std::filesystem::path& confPath);
    std::string readNextTokenAsString();
    float readNextTokenAsFloat();
    glm::mat4 parseTransform();
    float parseValue(const PropertyType& propertyType);
    float parseBinaryValue(const PropertyType& propertyType);
    float convertColorValue(float value, const std::string& type);
    void clearHeader();
    void parseHeader();
    void parseProperty(const PropertyType& propertyType, Vertex& vertex);
    void readFileContent(const std::filesystem::path& filePath);
    std::unique_ptr<Object3D> createObject3D(const std::vector<Vertex>& vertices) const;
    std::vector<Vertex> scansToPointCloud(
      const std::vector<std::vector<Vertex>>& scans
    ) const;

   private:
    Header mHeader;
    ModelLoaderConfig mConfig;
  };
}
