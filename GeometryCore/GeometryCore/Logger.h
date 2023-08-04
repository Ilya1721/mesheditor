#pragma once

#include <string>

#include <glm/glm.hpp>

namespace GeometryCore
{
	std::string getMatrixPrettyString(const glm::mat4& mat);
	std::string getVectorPrettyString(const glm::vec2& vec);
	std::string getVectorPrettyString(const glm::vec3& vec);
	std::string getVectorPrettyString(const glm::vec4& vec);
}

