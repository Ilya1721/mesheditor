#include "STLModelLoader.h"

#include "Constants.h"
#include "Utility/FileHelper.h"
#include "Utility/StringHelper.h"

using namespace Utility;

namespace
{
  glm::vec3 readBinaryXYZ(const char*& buffer)
  {
    glm::vec3 xyz {};
    for (size_t coordIdx = 0; coordIdx < 3; ++coordIdx)
    {
      xyz[coordIdx] = *reinterpret_cast<const float*>(buffer);
      buffer += sizeof(float);
    }

    return xyz;
  }
}  // namespace

namespace RenderSystem
{
  std::unique_ptr<Object3D> STLModelLoader::loadModel(
    const std::filesystem::path& filePath
  )
  {
    mFileContent = readFile(filePath);
    if (mFileContent.empty())
    {
      throw std::exception("File content is empty");
    }

    return isBinary() ? loadBinary() : loadText();
  }

  bool STLModelLoader::isBinary() const
  {
    const char* buffer = mFileContent.c_str();
    buffer += STL_HEADER_SIZE;

    auto numberOfTriangles = *reinterpret_cast<const uint32_t*>(buffer);
    auto correctBinaryFileSize = numberOfTriangles * 50 + 84;

    return correctBinaryFileSize == mFileContent.size();
  }

  std::unique_ptr<Object3D> STLModelLoader::loadBinary()
  {
    const char* buffer = mFileContent.c_str();
    buffer += STL_HEADER_SIZE;

    auto facesCount = *reinterpret_cast<const uint32_t*>(buffer);
    buffer += sizeof(uint32_t);

    std::vector<MeshCore::Vertex> vertices;
    for (size_t faceIdx = 0; faceIdx < facesCount; ++faceIdx)
    {
      auto faceNormal = readBinaryXYZ(buffer);
      for (int vertexIdx = 0; vertexIdx < 3; ++vertexIdx)
      {
        auto pos = readBinaryXYZ(buffer);
        vertices.push_back({pos, faceNormal});
      }
      buffer += sizeof(uint16_t);
    }

    auto mesh = std::make_unique<MeshCore::Mesh>(vertices);
    return std::make_unique<Object3D>(std::move(mesh), EMERALD_MATERIAL);
  }

  std::unique_ptr<Object3D> STLModelLoader::loadText()
  {
    std::vector<MeshCore::Vertex> vertices;
    glm::vec3 faceNormal {};
    mContext = nullptr;
    mCurrentToken = strtok_s(mFileContent.data(), DELIMITERS.c_str(), &mContext);

    while (mCurrentToken != nullptr)
    {
      if (isEqual(mCurrentToken, "normal"))
      {
        faceNormal = readXYZ();
      }
      else if (isEqual(mCurrentToken, "vertex"))
      {
        vertices.push_back({readXYZ(), faceNormal});
      }
      mCurrentToken = strtok_s(nullptr, DELIMITERS.c_str(), &mContext);
    }

    auto mesh = std::make_unique<MeshCore::Mesh>(vertices);
    return std::make_unique<Object3D>(std::move(mesh), EMERALD_MATERIAL);
  }
}  // namespace RenderSystem