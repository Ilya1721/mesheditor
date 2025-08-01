#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Utility
{
  bool isEqual(
    const std::string& firstString,
    const std::string& secondString,
    bool caseSensitive = false
  );
  std::string getMatrixPrettyString(const glm::mat4& mat);
  std::string getVectorPrettyString(const glm::vec2& vec);
  std::string getVectorPrettyString(const glm::vec3& vec);
  std::string getVectorPrettyString(const glm::vec4& vec);
}  // namespace Utility
