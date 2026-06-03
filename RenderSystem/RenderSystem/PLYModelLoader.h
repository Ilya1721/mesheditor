#pragma once

#include <filesystem>

#include "MeshCore/PointCloud.h"
#include "ModelLoaderUtils.h"

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
    PointCloud loadPointCloud(const std::filesystem::path& filePath);

   private:
    PointCloud parseVertices();
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
