module;
#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"
module BasicRenderer;

import <iostream>;

import FileHelper;

namespace RenderSystem
{
	BasicRenderer::BasicRenderer() noexcept :
		mVertexShader(0),
		mFragmentShader(0),
		mShaderProgram(0),
		mMVP(0)
	{}

	BasicRenderer::~BasicRenderer() noexcept
	{
		glUseProgram(0);
		glDeleteProgram(mVertexShader);
		glDeleteProgram(mFragmentShader);
		glDeleteProgram(mShaderProgram);
	}

	void BasicRenderer::init() noexcept
	{
		mVertexShader = loadShader(R"(../RenderSystem/Shaders/VertexShader.vert)", GL_VERTEX_SHADER);
		mFragmentShader = loadShader(R"(../RenderSystem/Shaders/FragmentShader.frag)", GL_FRAGMENT_SHADER);

		mShaderProgram = glCreateProgram();
		glAttachShader(mShaderProgram, mVertexShader);
		glAttachShader(mShaderProgram, mFragmentShader);
		glLinkProgram(mShaderProgram);

		int isLinked = false;
		glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &isLinked);
		if (!isLinked)
		{
			const auto& shaderLog = getShaderLog(mShaderProgram, SHADER_LOG_TYPE::SHADER_PROGRAM);
			std::cerr << "Shader program was not linked\n" << shaderLog.data() << std::endl;
			glDeleteProgram(mShaderProgram);
			return;
		}

		mMVP = glGetUniformLocation(mShaderProgram, "mvp");
		beginUse();
	}

	int BasicRenderer::getBasicVertexShader() const noexcept
	{
		return mVertexShader;
	}

	int BasicRenderer::getBasicFragmentShader() const noexcept
	{
		return mFragmentShader;
	}

	void BasicRenderer::beginUse() noexcept
	{
		glUseProgram(mShaderProgram);
	}

	void BasicRenderer::endUse() noexcept
	{
		glUseProgram(0);
	}

	void BasicRenderer::setMVP(const double* mvp) noexcept
	{
		glUniformMatrix4dv(mMVP, 1, false, mvp);
	}

	int BasicRenderer::loadShader(const std::string& shaderPath, int shaderType) noexcept
	{
		auto shader = glCreateShader(shaderType);
		auto shaderString = Helpers::readFile(shaderPath);
		auto shaderStr = shaderString.c_str();

		glShaderSource(shader, 1, &shaderStr, 0);
		glCompileShader(shader);

		int isCompiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		printOpenGLErrorMessage();

		if (!isCompiled)
		{
			auto shaderLog = getShaderLog(shader, SHADER_LOG_TYPE::SHADER);
			std::cerr << "Shader was not compiled!\n" << shaderLog.data() << std::endl;
			glDeleteShader(shader);
			return -1;
		}

		return shader;
	}

	std::vector<char> BasicRenderer::getShaderLog(int shaderOrProgramId, SHADER_LOG_TYPE logType) noexcept
	{
		int logLength = 0;
		std::vector<char> errorLog {'\0'};

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

	void BasicRenderer::printOpenGLErrorMessage() noexcept
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
