#pragma once

#include <filesystem>

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
      bool isBinary;
      size_t vertexCount;
      std::vector<PropertyType> propertyTypes;
    };

   public:
    std::unique_ptr<Object3D> loadPointCloud(const std::filesystem::path& filePath);
    std::unique_ptr<Object3D> loadMultiplePointClouds(
      const std::filesystem::path& folderPath
    );

   private:
    std::vector<Vertex> parseVertices();
    std::string readNextTokenAsString();
    float parseValue(const PropertyType& propertyType);
    float parseTextValue(const PropertyType& propertyType);
    float parseBinaryValue(const PropertyType& propertyType);
    float convertColorValue(float value, const std::string& type);
    void parseHeader();
    void parseProperty(const PropertyType& propertyType, Vertex& vertex);

   private:
    Header mHeader;
  };
}
