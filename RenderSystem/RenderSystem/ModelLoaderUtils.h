#pragma once

#include <glm/glm.hpp>
#include <string>

namespace RenderSystem
{
  class ModelLoaderUtils
  {
   protected:
    glm::vec3 readXYZ(int dimensions = 3);

   protected:
    std::string mFileContent;
    char* mCurrentToken;
    char* mContext;
  };
}
