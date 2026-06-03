#include "PLYModelLoader.h"

#include "Constants.h"
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
  PointCloud PLYModelLoader::loadPointCloud(const std::filesystem::path& filePath)
  {
    mFileContent = readFile(filePath);
    if (mFileContent.empty())
    {
      throw std::exception("File content is empty");
    }

    mContext = nullptr;
    mCurrentToken = strtok_s(mFileContent.data(), DELIMITERS.c_str(), &mContext);
    parseHeader();

    return parseVertices();
  }

  PointCloud PLYModelLoader::parseVertices()
  {
    PointCloud pointCloud;
    for (size_t vIdx = 0; vIdx < mHeader.vertexCount; ++vIdx)
    {
      Vertex vertex {};
      for (const auto& propertyType : mHeader.propertyTypes)
      {
        parseProperty(propertyType, vertex);
      }
      pointCloud.push_back(vertex);
    }

    return pointCloud;
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
