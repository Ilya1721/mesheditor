#include "ObjModelLoader.h"

#include <numeric>

#include "Constants.h"
#include "Utility/FileHelper.h"
#include "Utility/StringHelper.h"
#include "TextureFactory.h"

using namespace Utility;

namespace
{
  using namespace RenderSystem;

  std::string getExtension(const std::string& path)
  {
    auto dotPos = path.find_last_of('.');
    if (dotPos == std::string::npos)
    {
      return "";
    }
    return path.substr(dotPos);
  }

  bool isPathAbsolute(const std::string& path)
  {
    return path.size() > 2 && path[1] == ':';
  }
}  // namespace

namespace RenderSystem
{
  BlinnPhongMaterial ObjMaterialLoader::load(const std::filesystem::path& filePath)
  {
    mFileContent = Utility::readFile(filePath);
    if (mFileContent.empty())
    {
      throw std::exception("Could not parse the .mtl file");
    }

    BlinnPhongMaterial material;
    mContext = nullptr;
    mCurrentToken = strtok_s(mFileContent.data(), DELIMITERS.c_str(), &mContext);

    while (mCurrentToken != nullptr)
    {
      if (Utility::isEqual(mCurrentToken, "Ns"))
      {
        mCurrentToken = strtok_s(nullptr, DELIMITERS.c_str(), &mContext);
        material.shininess = std::stof(mCurrentToken);
      }
      else if (Utility::isEqual(mCurrentToken, "Ka"))
      {
        material.ambient = readXYZ();
      }
      else if (Utility::isEqual(mCurrentToken, "Kd"))
      {
        material.diffuse = readXYZ();
      }
      else if (Utility::isEqual(mCurrentToken, "Ks"))
      {
        material.specular = readXYZ();
      }
      else if (Utility::isEqual(mCurrentToken, "map_Kd"))
      {
        material.diffuseTexture = parseTexture(filePath);
      }
      mCurrentToken = strtok_s(nullptr, DELIMITERS.c_str(), &mContext);
    }

    return material;
  }

  std::shared_ptr<Texture2D> ObjMaterialLoader::parseTexture(
    const std::filesystem::path& filePath
  )
  {
    auto texturePath = parseTexturePath();
    std::filesystem::path fullTexturePath;
    if (isPathAbsolute(texturePath))
    {
      fullTexturePath = texturePath;
    }
    else
    {
      fullTexturePath = filePath.parent_path() / std::filesystem::path(texturePath);
    }

    return createImageTexture(fullTexturePath.string());
  }

  std::string ObjMaterialLoader::parseTexturePath()
  {
    std::vector<std::string> pathParts;
    while (SUPPORTED_TEXTURE_EXTENSIONS.find(getExtension(mCurrentToken)) ==
           SUPPORTED_TEXTURE_EXTENSIONS.end())
    {
      mCurrentToken = strtok_s(nullptr, DELIMITERS.c_str(), &mContext);
      pathParts.push_back(mCurrentToken);
    }

    auto beginIt = pathParts[0] == "." ? pathParts.begin() + 1 : pathParts.begin();

    return std::accumulate(
      std::next(beginIt), pathParts.end(), *beginIt,
      [](const std::string& a, const std::string& b) { return a + "\\" + b; }
    );
  }

  std::unique_ptr<Object3D> OBJModelLoader::load(const std::filesystem::path& filePath)
  {
    mFileContent = Utility::readFile(filePath);
    if (mFileContent.empty())
    {
      throw std::exception("Could not parse the .obj file");
    }

    return loadText(filePath);
  }

  std::unique_ptr<Object3D> OBJModelLoader::loadText(const std::filesystem::path& filePath
  )
  {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textures;
    std::vector<glm::vec3> normals;
    std::vector<MeshCore::Vertex> vertices;
    BlinnPhongMaterial material;

    mContext = nullptr;
    mCurrentToken = strtok_s(mFileContent.data(), DELIMITERS.c_str(), &mContext);

    while (mCurrentToken != nullptr)
    {
      if (isEqual(mCurrentToken, "v"))
      {
        positions.push_back(readXYZ());
      }
      else if (isEqual(mCurrentToken, "vt"))
      {
        textures.push_back(readXYZ(2));
      }
      else if (isEqual(mCurrentToken, "vn"))
      {
        normals.push_back(readXYZ());
      }
      else if (isEqual(mCurrentToken, "f"))
      {
        parseFaceOBJ(vertices, positions, textures, normals);
      }
      else if (isEqual(mCurrentToken, "mtllib"))
      {
        auto mtlFileName = parseMaterialFileName();
        auto mtlFilePath = filePath.parent_path() / mtlFileName;
        ObjMaterialLoader materialLoader;
        material = materialLoader.load(mtlFilePath);
      }
      mCurrentToken = strtok_s(nullptr, DELIMITERS.c_str(), &mContext);
    }

    auto mesh = std::make_unique<MeshCore::Mesh>(vertices);
    return std::make_unique<Object3D>(std::move(mesh), material);
  }

  void OBJModelLoader::parseFaceOBJ(
    std::vector<MeshCore::Vertex>& vertices,
    const std::vector<glm::vec3>& positions,
    const std::vector<glm::vec2>& textures,
    const std::vector<glm::vec3>& normals
  )
  {
    std::vector<MeshCore::Vertex> tempVertices;
    while (mContext[0] != 'f' && mContext[0] != '\0')
    {
      std::vector<size_t> vertexIndices;
      for (size_t i = 0; i < 3; ++i)
      {
        mCurrentToken = strtok_s(nullptr, DELIMITERS.c_str(), &mContext);
        vertexIndices.push_back(std::stoul(mCurrentToken) - 1ull);
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

  std::string OBJModelLoader::parseMaterialFileName()
  {
    std::vector<std::string> nameParts;
    while (!std::string(mCurrentToken).ends_with(".mtl"))
    {
      mCurrentToken = strtok_s(nullptr, DELIMITERS.c_str(), &mContext);
      nameParts.push_back(mCurrentToken);
    }

    return std::accumulate(
      std::next(nameParts.begin()), nameParts.end(), nameParts[0],
      [](const std::string& a, const std::string& b) { return a + " " + b; }
    );
  }
}  // namespace RenderSystem
