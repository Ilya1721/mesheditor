#include "StringHelper.h"

#include <glm/gtc/type_ptr.hpp>
#include <sstream>

namespace Utility
{
  bool isEqual(
    const std::string& firstString, const std::string& secondString, bool caseSensitive
  )
  {
    return std::equal(
      firstString.begin(), firstString.end(), secondString.begin(), secondString.end(),
      [caseSensitive](char first, char second)
      {
        return caseSensitive ? first == second
                             : std::tolower(first) == std::tolower(second);
      }
    );
  }

  std::string getMatrixPrettyString(const glm::mat4& mat)
  {
    std::stringstream outputStream;
    outputStream << "{";

    for (size_t rowIdx = 0; rowIdx < 4; ++rowIdx)
    {
      outputStream << "\n\t";
      for (size_t colIdx = 0; colIdx < 4; ++colIdx)
      {
        outputStream << std::to_string(glm::value_ptr(mat)[rowIdx * 4 + colIdx]) << " ";
      }
    }

    outputStream << "\n}";

    return outputStream.str();
  }

  std::string getVectorPrettyString(const glm::vec2& vec)
  {
    return getVectorPrettyString(glm::vec3(vec, 0.0f));
  }

  std::string getVectorPrettyString(const glm::vec3& vec)
  {
    return getVectorPrettyString(glm::vec4(vec, 0.0f));
  }

  std::string getVectorPrettyString(const glm::vec4& vec)
  {
    std::stringstream outputStream;
    outputStream << "{";

    for (glm::length_t vecIdx = 0; vecIdx < 4; ++vecIdx)
    {
      outputStream << " " << std::to_string(vec[vecIdx]);
    }

    outputStream << " }";

    return outputStream.str();
  }
}  // namespace Utility
