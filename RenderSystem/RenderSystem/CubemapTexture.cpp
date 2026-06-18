#include "CubemapTexture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "ImageLoader.h"
#include "TextureUtils.h"

namespace RenderSystem
{
  CubemapTexture::CubemapTexture(const std::array<path, 6>& cubemapFaces) : mTexture(0)
  {
    glGenTextures(1, &mTexture);
    loadCubemapFaces(cubemapFaces);
    initParameters();
  }

  unsigned int CubemapTexture::getId() const
  {
    return mTexture;
  }

  void CubemapTexture::bind() const
  {
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
  }

  void CubemapTexture::initParameters() const
  {
    bind();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  }

  void CubemapTexture::loadCubemapFaces(const std::array<path, 6>& faces)
  {
    for (unsigned int faceIdx = 0; faceIdx < faces.size(); ++faceIdx)
    {
      ImageLoader face(faces[faceIdx].string());
      face.load();
      createCubemapTexture(
        face.getWidth(), face.getHeight(), face.getData(), face.getColorChannels(),
        faceIdx
      );
    }
  }

  void CubemapTexture::createCubemapTexture(
    int width, int height, const unsigned char* data, int colorChannels, int faceIdx
  ) const
  {
    bind();
    const auto colorFormat = getColorFormat(colorChannels);
    glTexImage2D(
      GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIdx, 0, colorFormat, width, height, 0,
      colorFormat, GL_UNSIGNED_BYTE, data
    );
  }
}  // namespace RenderSystem
