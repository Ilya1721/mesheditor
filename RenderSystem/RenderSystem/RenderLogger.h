#pragma once

#include <string>

namespace RenderSystem
{
	enum class SHADER_TYPE
	{
		SHADER,
		SHADER_PROGRAM
	};

	std::string getShaderInfoLog(int shaderId, SHADER_TYPE shaderType);
	void printOpenGLErrorMessage();
}

