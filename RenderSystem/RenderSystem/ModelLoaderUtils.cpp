#include "ModelLoaderUtils.h"

#include "Constants.h"

namespace RenderSystem
{
  glm::vec3 ModelLoaderUtils::readXYZ(int dimensions)
  {
    glm::vec3 xyz {};
    for (int coordIdx = 0; coordIdx < dimensions; ++coordIdx)
    {
      mCurrentToken = strtok_s(nullptr, DELIMITERS.c_str(), &mContext);
      xyz[coordIdx] = std::stof(mCurrentToken);
    }

    return xyz;
  }
}
