#include "MeshFilesLoader.h"

#include <numeric>

#include "Constants.h"
#include "GeometryCore/Typedefs.h"
#include "MeshCore/Constants.h"
#include "MeshCore/Mesh.h"
#include "Utility/FileHelper.h"
#include "Utility/StringHelper.h"

namespace
{
  using parseTokenFunc =
    void(char*& currentToken, char*& context, const char* delimeters);

  using namespace GeometryCore;

  bool isBinarySTL(const std::string& fileContent)
  {
    if (fileContent.empty()) { throw std::exception("File content is empty"); }

    const char* buffer = fileContent.c_str();
    buffer += MeshFilesLoader::STL_HEADER_SIZE;

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

  void readCoordinatesFromBuffer(glm::vec3& coordinates, const char*& buffer)
  {
    for (int coordIdx = 0; coordIdx < 3; ++coordIdx)
    {
      coordinates[coordIdx] = *reinterpret_cast<const float*>(buffer);
      buffer += sizeof(float);
    }
  }

  void parseText(
    std::string& fileContent, const std::function<parseTokenFunc>& parseToken
  )
  {
    char* context = nullptr;
    auto delimiters = MeshFilesLoader::DELIMITERS.c_str();
    char* currentToken = strtok_s(fileContent.data(), delimiters, &context);

    while (currentToken != nullptr)
    {
      parseToken(currentToken, context, delimiters);
      currentToken = strtok_s(nullptr, delimiters, &context);
    }
  }

  std::unique_ptr<MeshCore::Mesh> parseTextSTL(std::string& fileContent)
  {
    std::vector<MeshCore::Vertex> vertices;
    Vector3D faceNormal {};

    parseText(
      fileContent,
      [&vertices,
       &faceNormal](char*& currentToken, char*& context, const char* delimiters)
      {
        if (Utility::isEqual(currentToken, "normal"))
        {
          readTokenAsVector(currentToken, delimiters, context, faceNormal);
        }
        else if (Utility::isEqual(currentToken, "vertex"))
        {
          Point3D pos {};
          readTokenAsVector(currentToken, delimiters, context, pos);
          vertices.push_back({pos, faceNormal});
        }
      }
    );

    return std::make_unique<MeshCore::Mesh>(vertices);
  }

  std::unique_ptr<MeshCore::Mesh> parseBinarySTL(const std::string& fileContent)
  {
    const char* buffer = fileContent.c_str();
    buffer += MeshFilesLoader::STL_HEADER_SIZE;

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

    return std::make_unique<MeshCore::Mesh>(vertices);
  }

  void parseFaceOBJ(
    std::vector<MeshCore::Vertex>& vertices,
    const std::vector<Point3D>& positions,
    const std::vector<Point2D>& textures,
    const std::vector<Vector3D>& normals,
    char*& currentToken,
    char*& context,
    const char* delimiters
  )
  {
    std::vector<MeshCore::Vertex> tempVertices;
    while (context[0] != 'f' && context[0] != '\0')
    {
      std::vector<size_t> vertexIndices;
      for (size_t i = 0; i < 3; ++i)
      {
        currentToken = strtok_s(nullptr, delimiters, &context);
        vertexIndices.push_back(std::stoul(currentToken) - 1ull);
      }
      MeshCore::Vertex vertex;
      vertex.pos = positions[vertexIndices[0]];
      vertex.texture = textures[vertexIndices[1]];
      vertex.normal = normals[vertexIndices[2]];
      tempVertices.push_back(vertex);
    }

    if (tempVertices.size() == 3)
    {
      vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
    }
    else if (tempVertices.size() == 4)
    {
      vertices.push_back(tempVertices[0]);
      vertices.push_back(tempVertices[1]);
      vertices.push_back(tempVertices[2]);
      vertices.push_back(tempVertices[0]);
      vertices.push_back(tempVertices[2]);
      vertices.push_back(tempVertices[3]);
    }
  }

  std::string parseTextureRelativePath(
    char*& currentToken, char*& context, const char* delimiters
  )
  {
    std::vector<std::string> pathParts;
    while (!std::string(currentToken).ends_with(".png"))
    {
      currentToken = strtok_s(nullptr, delimiters, &context);
      pathParts.push_back(currentToken);
    }

    return std::accumulate(
      std::next(pathParts.begin() + 1), pathParts.end(), pathParts[1],
      [](const std::string& a, const std::string& b) { return a + "\\" + b; }
    );
  }

  std::string parseMaterialFileName(
    char*& currentToken, char*& context, const char* delimiters
  )
  {
    std::vector<std::string> nameParts;
    while (!std::string(currentToken).ends_with(".mtl"))
    {
      currentToken = strtok_s(nullptr, delimiters, &context);
      nameParts.push_back(currentToken);
    }

    return std::accumulate(
      std::next(nameParts.begin()), nameParts.end(), nameParts[0],
      [](const std::string& a, const std::string& b) { return a + " " + b; }
    );
  }

  template <typename T>
  void addParsedVecToArray(
    std::vector<T>& arr,
    char*& currentToken,
    char*& context,
    const char* delimiters,
    int dimensions = 3
  )
  {
    T vec {};
    readTokenAsVector(currentToken, delimiters, context, vec, dimensions);
    arr.push_back(vec);
  }

  void parseTextMTL(
    const std::filesystem::path& filePath, MeshCore::MaterialParams& materialParams
  )
  {
    auto fileContent = Utility::readFile(filePath);
    parseText(
      fileContent,
      [&materialParams,
       &filePath](char*& currentToken, char*& context, const char* delimiters)
      {
        if (Utility::isEqual(currentToken, "Ns"))
        {
          currentToken = strtok_s(nullptr, delimiters, &context);
          materialParams.shininess = std::stof(currentToken);
        }
        else if (Utility::isEqual(currentToken, "Ka"))
        {
          readTokenAsVector(currentToken, delimiters, context, materialParams.ambient, 3);
        }
        else if (Utility::isEqual(currentToken, "Kd"))
        {
          readTokenAsVector(currentToken, delimiters, context, materialParams.diffuse, 3);
        }
        else if (Utility::isEqual(currentToken, "Ks"))
        {
          readTokenAsVector(
            currentToken, delimiters, context, materialParams.specular, 3
          );
        }
        else if (Utility::isEqual(currentToken, "map_Kd"))
        {
          auto textureRelativePath =
            parseTextureRelativePath(currentToken, context, delimiters);
          materialParams.diffuseTexturePath =
            filePath.parent_path() / std::filesystem::path(textureRelativePath);
        }
      }
    );
  }

  std::unique_ptr<MeshCore::Mesh> parseTextOBJ(const std::filesystem::path& filePath)
  {
    auto fileContent = Utility::readFile(filePath);
    std::vector<Point3D> positions;
    std::vector<Point2D> textures;
    std::vector<Vector3D> normals;
    std::vector<MeshCore::Vertex> vertices;
    MeshCore::MaterialParams materialParams = MeshCore::GOLD_MATERIAL;

    parseText(
      fileContent,
      [&positions, &normals, &textures, &vertices, &filePath,
       &materialParams](char*& currentToken, char*& context, const char* delimiters)
      {
        if (Utility::isEqual(currentToken, "v"))
        {
          addParsedVecToArray(positions, currentToken, context, delimiters);
        }
        else if (Utility::isEqual(currentToken, "vt"))
        {
          addParsedVecToArray(textures, currentToken, context, delimiters, 2);
        }
        else if (Utility::isEqual(currentToken, "vn"))
        {
          addParsedVecToArray(normals, currentToken, context, delimiters);
        }
        else if (Utility::isEqual(currentToken, "f"))
        {
          parseFaceOBJ(
            vertices, positions, textures, normals, currentToken, context, delimiters
          );
        }
        else if (Utility::isEqual(currentToken, "mtllib"))
        {
          auto mtlFileName = parseMaterialFileName(currentToken, context, delimiters);
          auto mtlFilePath = filePath.parent_path() / mtlFileName;
          parseTextMTL(mtlFilePath, materialParams);
        }
      }
    );

    return std::make_unique<MeshCore::Mesh>(vertices, materialParams);
  }

  std::unique_ptr<MeshCore::Mesh> parseSTL(const std::filesystem::path& filePath)
  {
    auto fileContent = Utility::readFile(filePath);
    return isBinarySTL(fileContent) ? parseBinarySTL(fileContent)
                                    : parseTextSTL(fileContent);
  }
}  // namespace

namespace MeshFilesLoader
{
  std::unique_ptr<MeshCore::Mesh> loadMeshFromFile(const std::filesystem::path& filePath)
  {
    if (Utility::isEqual(filePath.extension().string(), ".stl"))
    {
      return parseSTL(filePath);
    }
    else if (Utility::isEqual(filePath.extension().string(), ".obj"))
    {
      return parseTextOBJ(filePath);
    }
    else { throw std::exception("Unsupported file format"); }
  }
}  // namespace MeshFilesLoader