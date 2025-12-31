#include "STLModelLoader.h"

#include "Constants.h"
#include "Utility/FileHelper.h"
#include "Utility/StringHelper.h"

using namespace Utility;

namespace
{
  using namespace RenderSystem;

  const BlinnPhongMaterial& DEFAULT_MATERIAL = EMERALD_MATERIAL;

  using parseTokenFunc =
    void(char*& currentToken, char*& context, const char* delimeters);

  void readCoordinatesFromBuffer(glm::vec3& coordinates, const char*& buffer)
  {
    for (int coordIdx = 0; coordIdx < 3; ++coordIdx)
    {
      coordinates[coordIdx] = *reinterpret_cast<const float*>(buffer);
      buffer += sizeof(float);
    }
  }

  std::unique_ptr<Object3D> loadBinarySTL(const std::string& fileContent)
  {
    const char* buffer = fileContent.c_str();
    buffer += STL_HEADER_SIZE;

    auto facesCount = *reinterpret_cast<const uint32_t*>(buffer);
    buffer += sizeof(uint32_t);

    std::vector<MeshCore::Vertex> vertices;
    for (size_t faceIdx = 0; faceIdx < facesCount; ++faceIdx)
    {
      Vector3D faceNormal {};
      readCoordinatesFromBuffer(faceNormal, buffer);

      for (int vertexIdx = 0; vertexIdx < 3; ++vertexIdx)
      {
        Point3D pos {};
        readCoordinatesFromBuffer(pos, buffer);
        vertices.push_back({pos, faceNormal});
      }

      buffer += sizeof(uint16_t);
    }

    auto mesh = std::make_unique<MeshCore::Mesh>(vertices);

    return std::make_unique<Object3D>(std::move(mesh), DEFAULT_MATERIAL);
  }

  bool isBinarySTL(const std::string& fileContent)
  {
    if (fileContent.empty()) { throw std::exception("File content is empty"); }

    const char* buffer = fileContent.c_str();
    buffer += STL_HEADER_SIZE;

    auto numberOfTriangles = *reinterpret_cast<const uint32_t*>(buffer);
    auto correctBinaryFileSize = numberOfTriangles * 50 + 84;

    return correctBinaryFileSize == fileContent.size();
  }

  template <typename Vec>
  void readTokenAsVector(
    char*& currentToken,
    const char* delimiters,
    char*& context,
    Vec& coordinates,
    int dimensions = 3
  )
  {
    for (int coordIdx = 0; coordIdx < dimensions; ++coordIdx)
    {
      currentToken = strtok_s(nullptr, delimiters, &context);
      coordinates[coordIdx] = std::stof(currentToken);
    }
  }

  void parseText(
    std::string& fileContent, const std::function<parseTokenFunc>& parseToken
  )
  {
    char* context = nullptr;
    auto delimiters = DELIMITERS.c_str();
    char* currentToken = strtok_s(fileContent.data(), delimiters, &context);

    while (currentToken != nullptr)
    {
      parseToken(currentToken, context, delimiters);
      currentToken = strtok_s(nullptr, delimiters, &context);
    }
  }

  std::unique_ptr<Object3D> loadTextSTL(std::string& fileContent)
  {
    std::vector<MeshCore::Vertex> vertices;
    Vector3D faceNormal{};

    parseText(
      fileContent,
      [&vertices,
      &faceNormal](char*& currentToken, char*& context, const char* delimiters)
      {
        if (isEqual(currentToken, "normal"))
        {
          readTokenAsVector(currentToken, delimiters, context, faceNormal);
        }
        else if (isEqual(currentToken, "vertex"))
        {
          Point3D pos{};
          readTokenAsVector(currentToken, delimiters, context, pos);
          vertices.push_back({ pos, faceNormal });
        }
      }
    );

    auto mesh = std::make_unique<MeshCore::Mesh>(vertices);

    return std::make_unique<Object3D>(std::move(mesh), DEFAULT_MATERIAL);
  }
}  // namespace

namespace RenderSystem
{
  std::unique_ptr<Object3D> loadSTLModel(const std::filesystem::path& filePath)
  {
    auto fileContent = readFile(filePath);
    return isBinarySTL(fileContent) ? loadBinarySTL(fileContent)
                                    : loadTextSTL(fileContent);
  }
}  // namespace RenderSystem