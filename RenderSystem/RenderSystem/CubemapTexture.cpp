#include "CubemapTexture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <stb_image/stb_image.h>

#include "glad/glad.h"

namespace RenderSystem
{
  CubemapTexture::CubemapTexture(const std::array<path, 6>& cubemapFaces)
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
    invoke(
      [&faces]()
      {
        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); ++i)
        {
          unsigned char* data =
            stbi_load(faces[i].string().c_str(), &width, &height, &nrChannels, 0);
          if (data)
          {
            glTexImage2D(
              GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB,
              GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
          }
          else { stbi_image_free(data); }
        }
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

  void CubemapTexture::passToFragmentShader(int textureLocation) const
  {
    glActiveTexture(GL_TEXTURE0 + textureLocation);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
    glUniform1i(textureLocation, mTexture);
  }
}  // namespace RenderSystem
