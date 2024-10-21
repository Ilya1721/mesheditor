#include "ShaderProgram.h"

#include <iostream>

#include "Utility/FileHelper.h"
#include "Utility/StringHelper.h"

#include "GladTypedefs.h"
#include "RenderLogger.h"

namespace
{
	using namespace RenderSystem;

	void checkShaderOrProgramStatus(GetShaderIV getShaderIVFunc, int shaderOrProgram, int statusToCheck,
		SHADER_TYPE shaderType, const std::string& failedMessage)
	{
		int isStatusSuccessful = false;
		getShaderIVFunc(shaderOrProgram, statusToCheck, &isStatusSuccessful);
		if (!isStatusSuccessful)
		{
			auto shaderLog = getShaderInfoLog(shaderOrProgram, shaderType);
			std::cerr << shaderLog << std::endl;
			throw std::exception(failedMessage.c_str());
		}
	}

	int loadShader(const std::string& shaderPath, int shaderType)
	{
		auto shaderContent = Utility::readFile(shaderPath);
		std::vector<const char*> shaderContentVec{ shaderContent.c_str() };
		std::vector<int> shaderContentLengthVec{ static_cast<int>(shaderContent.size()) };

		auto shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, shaderContentVec.data(), shaderContentLengthVec.data());
		glCompileShader(shader);
		checkShaderOrProgramStatus(glGetShaderiv, shader, GL_COMPILE_STATUS, SHADER_TYPE::SHADER, "Shader was not compiled!");

		return shader;
	}
}

namespace RenderSystem
{
	ShaderProgram::ShaderProgram(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath) :
		mVertexShaderPath(vertexShaderPath),
		mFragmentShaderPath(fragmentShaderPath),
		mVertexShader(),
		mFragmentShader(),
		mShaderProgram()
	{
		init();
	}

	ShaderProgram::~ShaderProgram()
    {
        glDeleteShader(mVertexShader);
        glDeleteShader(mFragmentShader);
        glDeleteProgram(mShaderProgram);
    }

	void ShaderProgram::invokeAction(const std::function<void()>& action)
	{
		GLint shaderProgramToRestore;
		glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgramToRestore);
		glUseProgram(mShaderProgram);
		action();
		glUseProgram(shaderProgramToRestore);
	}

	void ShaderProgram::init()
	{
		mVertexShader = loadShader(mVertexShaderPath.string(), GL_VERTEX_SHADER);
		mFragmentShader = loadShader(mFragmentShaderPath.string(), GL_FRAGMENT_SHADER);
		initShaderProgram();
	}

	void ShaderProgram::initShaderProgram()
	{
		mShaderProgram = glCreateProgram();
		glAttachShader(mShaderProgram, mVertexShader);
		glAttachShader(mShaderProgram, mFragmentShader);
		glLinkProgram(mShaderProgram);
		checkShaderOrProgramStatus(glGetProgramiv, mShaderProgram, GL_LINK_STATUS, SHADER_TYPE::SHADER_PROGRAM, "Shader program has not been linked");
		glUseProgram(mShaderProgram);
	}
}
