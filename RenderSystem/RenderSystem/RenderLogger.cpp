#include "RenderLogger.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include <iostream>

namespace RenderSystem
{
	std::string getLog(int shaderOrProgramId, SHADER_LOG_TYPE logType)
	{
		int logLength = 0;
		std::string errorLog {'\0'};

		if (logType == SHADER_LOG_TYPE::SHADER)
		{
			glGetShaderiv(shaderOrProgramId, GL_INFO_LOG_LENGTH, &logLength);
			errorLog.resize(logLength);
			glGetShaderInfoLog(shaderOrProgramId, logLength, &logLength, errorLog.data());
		}
		else if (logType == SHADER_LOG_TYPE::SHADER_PROGRAM)
		{
			glGetProgramiv(shaderOrProgramId, GL_INFO_LOG_LENGTH, &logLength);
			errorLog.resize(logLength);
			glGetProgramInfoLog(shaderOrProgramId, logLength, &logLength, errorLog.data());
		}

		return errorLog;
	}

	void printOpenGLErrorMessage()
	{
		GLenum error;

		while ((error = glGetError()) != GL_NO_ERROR)
		{
			switch (error)
			{
				case GL_INVALID_ENUM:
				{
					std::cerr << "An unacceptable value is specified for an enumerated argument" << std::endl;
					break;
				}
				case GL_INVALID_VALUE:
				{
					std::cerr << "A numeric argument is out of range" << std::endl;
					break;
				}
				case GL_INVALID_OPERATION:
				{
					std::cerr << "The specified operation is not allowed in the current state" << std::endl;
					break;
				}
				case GL_INVALID_FRAMEBUFFER_OPERATION:
				{
					std::cerr << "The framebuffer object is not complete" << std::endl;
					break;
				}
				case GL_OUT_OF_MEMORY:
				{
					std::cerr << "There is not enough memory left to execute the command" << std::endl;
					break;
				}
				case GL_STACK_UNDERFLOW:
				{
					std::cerr << "An attempt has been made to perform an operation that would cause an internal stack to underflow" << std::endl;
					break;
				}
				case GL_STACK_OVERFLOW:
				{
					std::cerr << "An attempt has been made to perform an operation that would cause an internal stack to overflow" << std::endl;
					break;
				}
			}
		}
	}
}
