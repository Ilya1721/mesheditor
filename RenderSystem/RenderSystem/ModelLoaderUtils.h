#pragma once

#include <glm/glm.hpp>
#include <string>

namespace RenderSystem
{
  struct PointCloudConfig
  {
    bool convertToMesh = false;
  };

  struct ModelLoaderConfig
  {
    PointCloudConfig pointCloud;
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
