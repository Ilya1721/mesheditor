#include "CubemapTexture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "ImageLoader.h"

namespace RenderSystem
{
  CubemapTexture::CubemapTexture(const std::array<path, 6>& cubemapFaces) : Texture()
  {
    loadCubemapFaces(cubemapFaces);
    initParameters();
  }

  void CubemapTexture::initParameters()
  {
    invoke(
      []()
      {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      }
    );
  }

  void CubemapTexture::loadCubemapFaces(const std::array<path, 6>& faces)
  {
    for (unsigned int faceIdx = 0; faceIdx < faces.size(); ++faceIdx)
    {
      loadImage(
        faces[faceIdx].string(),
        [faceIdx,
         this](int width, int height, const unsigned char* decodedData, int colorChannels)
        { createCubemapTexture(width, height, decodedData, colorChannels, faceIdx); }
      );
    }
  }

  void CubemapTexture::createCubemapTexture(
    int width, int height, const unsigned char* data, int colorChannels, int faceIdx
  )
  {
    invoke(
      [&width, &height, &data, &colorChannels, &faceIdx, this]()
      {
        const auto colorFormat = getColorFormat(colorChannels);
        glTexImage2D(
          GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIdx, 0, colorFormat, width, height, 0,
          colorFormat, GL_UNSIGNED_BYTE, data
        );
      }
    );
  }

  void CubemapTexture::bind() const
  {
    glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &mResourceToRestore);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
  }

  void CubemapTexture::unbind() const
  {
    glBindTexture(GL_TEXTURE_CUBE_MAP, mResourceToRestore);
  }

  void CubemapTexture::passToFragmentShader(int textureLocation, int textureSlot) const
  {
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
    glUniform1i(textureLocation, textureSlot);
  }
}  // namespace RenderSystem
