#pragma once

#include <string>

namespace RenderSystem
{
	enum class SHADER_LOG_TYPE
	{
		SHADER,
		SHADER_PROGRAM
	};

	std::string getLog(int shaderOrProgramId, SHADER_LOG_TYPE logType);
	void printOpenGLErrorMessage();
}

