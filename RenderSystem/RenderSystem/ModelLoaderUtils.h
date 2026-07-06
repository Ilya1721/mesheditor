#pragma once

#include <glm/glm.hpp>
#include <string>

namespace RenderSystem
{
  struct ModelLoaderConfig
  {
    bool solidifyPointCloud = false;
  };

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
