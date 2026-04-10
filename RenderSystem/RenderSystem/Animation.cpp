#include "Animation.h"

namespace RenderSystem
{
  glm::mat4 calcJointLocalTransform(const Pose& pose)
  {
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), pose.translation);
    glm::mat4 rotation = glm::mat4_cast(pose.rotation);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), pose.scale);

    return translation * rotation * scale;
  }
}
