#include "ObjModelLoader.h"

#include <numeric>

#include "Constants.h"
#include "Utility/FileHelper.h"
#include "Utility/StringHelper.h"

using namespace Utility;

namespace
{
  using namespace RenderSystem;

  using parseTokenFunc =
    void(char*& currentToken, char*& context, const char* delimeters);

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

  std::string getExtension(const std::string& path)
  {
    auto dotPos = path.find_last_of('.');
    if (dotPos == std::string::npos) { return ""; }
    return path.substr(dotPos);
  }

  std::string parseTexturePath(
    char*& currentToken, char*& context, const char* delimiters
  )
  {
    std::vector<std::string> pathParts;
    while (SUPPORTED_TEXTURE_EXTENSIONS.find(getExtension(currentToken)) ==
           SUPPORTED_TEXTURE_EXTENSIONS.end())
    {
      currentToken = strtok_s(nullptr, delimiters, &context);
      pathParts.push_back(currentToken);
    }

    auto beginIt = pathParts[0] == "." ? pathParts.begin() + 1 : pathParts.begin();

    return std::accumulate(
      std::next(beginIt), pathParts.end(), *beginIt,
      [](const std::string& a, const std::string& b) { return a + "\\" + b; }
    );
  }

  bool isPathAbsolute(const std::string& path)
  {
    return path.size() > 2 && path[1] == ':';
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

  std::shared_ptr<ImageTexture> parseTexture(
    const std::filesystem::path& filePath,
    char*& currentToken,
    char*& context,
    const char* delimiters
  )
  {
    auto texturePath = parseTexturePath(currentToken, context, delimiters);
    std::filesystem::path fullTexturePath;
    if (isPathAbsolute(texturePath)) { fullTexturePath = texturePath; }
    else
    {
      fullTexturePath = filePath.parent_path() / std::filesystem::path(texturePath);
    }

    return std::make_shared<ImageTexture>(fullTexturePath.string());
  }

  void parseTextMTL(const std::filesystem::path& filePath, BlinnPhongMaterial& material)
  {
    auto fileContent = Utility::readFile(filePath);
    parseText(
      fileContent,
      [&material, &filePath](char*& currentToken, char*& context, const char* delimiters)
      {
        if (Utility::isEqual(currentToken, "Ns"))
        {
          currentToken = strtok_s(nullptr, delimiters, &context);
          material.shininess = std::stof(currentToken);
        }
        else if (Utility::isEqual(currentToken, "Ka"))
        {
          readTokenAsVector(currentToken, delimiters, context, material.ambient, 3);
        }
        else if (Utility::isEqual(currentToken, "Kd"))
        {
          readTokenAsVector(currentToken, delimiters, context, material.diffuse.rgb, 3);
        }
        else if (Utility::isEqual(currentToken, "Ks"))
        {
          readTokenAsVector(currentToken, delimiters, context, material.specular, 3);
        }
        else if (Utility::isEqual(currentToken, "map_Kd"))
        {
          material.diffuse.texture =
            parseTexture(filePath, currentToken, context, delimiters);
        }
      }
    );
  }

  std::unique_ptr<Object3D> loadTextOBJ(const std::filesystem::path& filePath)
  {
    auto fileContent = Utility::readFile(filePath);
    std::vector<Point3D> positions;
    std::vector<Point2D> textures;
    std::vector<Vector3D> normals;
    std::vector<MeshCore::Vertex> vertices;
    BlinnPhongMaterial material;

    parseText(
      fileContent,
      [&positions, &normals, &textures, &vertices, &filePath,
       &material](char*& currentToken, char*& context, const char* delimiters)
      {
        if (isEqual(currentToken, "v"))
        {
          addParsedVecToArray(positions, currentToken, context, delimiters);
        }
        else if (isEqual(currentToken, "vt"))
        {
          addParsedVecToArray(textures, currentToken, context, delimiters, 2);
        }
        else if (isEqual(currentToken, "vn"))
        {
          addParsedVecToArray(normals, currentToken, context, delimiters);
        }
        else if (isEqual(currentToken, "f"))
        {
          parseFaceOBJ(
            vertices, positions, textures, normals, currentToken, context, delimiters
          );
        }
        else if (isEqual(currentToken, "mtllib"))
        {
          auto mtlFileName = parseMaterialFileName(currentToken, context, delimiters);
          auto mtlFilePath = filePath.parent_path() / mtlFileName;
          parseTextMTL(mtlFilePath, material);
        }
      }
    );

    auto mesh = std::make_unique<MeshCore::Mesh>(vertices);

    return std::make_unique<Object3D>(std::move(mesh), material);
  }
}  // namespace

namespace RenderSystem
{
  std::unique_ptr<Object3D> loadOBJModel(const std::filesystem::path& filePath)
  {
    return loadTextOBJ(filePath);
  }
}  // namespace RenderSystem
