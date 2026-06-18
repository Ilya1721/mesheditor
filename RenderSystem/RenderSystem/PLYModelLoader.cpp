#include "PLYModelLoader.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Constants.h"
#include "Material.h"
#include "Utility/FileHelper.h"
#include "Utility/StringHelper.h"

using namespace Utility;

namespace
{
  template <typename T> float parseBinaryToken(char*& strPtr)
  {
    T value = *reinterpret_cast<const T*>(strPtr);
    strPtr += sizeof(T);
    return value;
  }
}

namespace RenderSystem
{
  std::unique_ptr<Object3D> PLYModelLoader::loadPointCloud(
    const std::filesystem::path& filePath
  )
  {
    mFileContent = readFile(filePath);
    if (mFileContent.empty())
    {
      throw std::exception("File content is empty");
    }

    mContext = nullptr;
    mCurrentToken = strtok_s(mFileContent.data(), DELIMITERS.c_str(), &mContext);
    parseHeader();
    const auto& vertices = parseVertices();
    auto mesh = std::make_unique<Mesh>(vertices, false);

    return std::make_unique<Object3D>(
      std::move(mesh), PointCloudMaterial {}, glm::mat4(1.0f), GL_POINTS
    );
  }

  std::unique_ptr<Object3D> PLYModelLoader::loadMultiplePointClouds(
    const std::filesystem::path& folderPath
  )
  {
    auto mesh = std::make_unique<Mesh>(std::vector<Vertex> {}, false);
    return std::make_unique<Object3D>(
      std::move(mesh), PointCloudMaterial {}, glm::mat4(1.0f), GL_POINTS
    );
  }

  std::vector<Vertex> PLYModelLoader::parseVertices()
  {
    std::vector<Vertex> vertices;
    for (size_t vIdx = 0; vIdx < mHeader.vertexCount; ++vIdx)
    {
      Vertex vertex {};
      for (const auto& propertyType : mHeader.propertyTypes)
      {
        parseProperty(propertyType, vertex);
      }
      vertices.push_back(vertex);
    }

    return vertices;
  }

  void PLYModelLoader::parseHeader()
  {
    while (mCurrentToken != nullptr)
    {
      if (strcmp(mCurrentToken, "format") == 0)
      {
        auto format = readNextTokenAsString();
        mHeader.isBinary = format.starts_with("binary");
      }
      if (strcmp(mCurrentToken, "vertex") == 0)
      {
        auto vertexCount = readNextTokenAsString();
        mHeader.vertexCount = std::stoull(vertexCount);
      }
      if (strcmp(mCurrentToken, "property") == 0)
      {
        auto type = readNextTokenAsString();
        auto property = readNextTokenAsString();
        if (type != "list")
        {
          mHeader.propertyTypes.emplace_back(type, property);
        }
      }
      if (std::string(mCurrentToken).starts_with("end_header"))
      {
        if (mHeader.isBinary)
        {
          mCurrentToken = strtok_s(nullptr, DELIMITERS.c_str(), &mContext);
        }
        break;
      }
      mCurrentToken = strtok_s(nullptr, DELIMITERS.c_str(), &mContext);
    }
  }

  std::string PLYModelLoader::readNextTokenAsString()
  {
    return strtok_s(nullptr, DELIMITERS.c_str(), &mContext);
  }

  float PLYModelLoader::parseBinaryValue(const PropertyType& propertyType)
  {
    if (propertyType.type == "char")
    {
      return parseBinaryToken<char>(mCurrentToken);
    }
    else if (propertyType.type == "uchar")
    {
      return parseBinaryToken<unsigned char>(mCurrentToken);
    }
    else if (propertyType.type == "short")
    {
      return parseBinaryToken<short>(mCurrentToken);
    }
    else if (propertyType.type == "short")
    {
      return parseBinaryToken<unsigned short>(mCurrentToken);
    }
    else if (propertyType.type == "int")
    {
      return parseBinaryToken<int>(mCurrentToken);
    }
    else if (propertyType.type == "uint")
    {
      return parseBinaryToken<unsigned int>(mCurrentToken);
    }
    else if (propertyType.type == "float")
    {
      return parseBinaryToken<float>(mCurrentToken);
    }
    else if (propertyType.type == "double")
    {
      return parseBinaryToken<double>(mCurrentToken);
    }

    throw std::exception("Unexpected .ply property type");
  }

  float PLYModelLoader::parseTextValue(const PropertyType& propertyType)
  {
    auto token = readNextTokenAsString();
    return std::stof(token);
  }

  float PLYModelLoader::parseValue(const PropertyType& propertyType)
  {
    if (mHeader.isBinary)
    {
      return parseBinaryValue(propertyType);
    }

    return parseTextValue(propertyType);
  }

  float PLYModelLoader::convertColorValue(float value, const std::string& type)
  {
    if (type == "float" || type == "double")
    {
      return value;
    }

    return value / 255.0f;
  }

  void PLYModelLoader::parseProperty(const PropertyType& propertyType, Vertex& vertex)
  {
    auto value = parseValue(propertyType);
    if (propertyType.property.starts_with("x"))
    {
      vertex.pos.x = value;
    }
    else if (propertyType.property.starts_with("y"))
    {
      vertex.pos.y = value;
    }
    else if (propertyType.property.starts_with("z"))
    {
      vertex.pos.z = value;
    }
    else if (propertyType.property.starts_with("nx"))
    {
      vertex.normal.x = value;
    }
    else if (propertyType.property.starts_with("ny"))
    {
      vertex.normal.y = value;
    }
    else if (propertyType.property.starts_with("nz"))
    {
      vertex.normal.z = value;
    }
    else if (propertyType.property.starts_with("r"))
    {
      vertex.color.r = convertColorValue(value, propertyType.type);
    }
    else if (propertyType.property.starts_with("g"))
    {
      vertex.color.g = convertColorValue(value, propertyType.type);
    }
    else if (propertyType.property.starts_with("b"))
    {
      vertex.color.b = convertColorValue(value, propertyType.type);
    }
  }
}
