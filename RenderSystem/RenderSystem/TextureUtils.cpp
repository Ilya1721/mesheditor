#include "TextureUtils.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

namespace RenderSystem
{
  int getColorFormat(int colorChannels)
  {
    switch (colorChannels)
    {
      case 1:
        return GL_RED;
      case 3:
        return GL_RGB;
      case 4:
        return GL_RGBA;
      default:
        return GL_RGB;
    }
  }
}