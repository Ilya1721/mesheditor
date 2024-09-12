#include "ShaderProgram.h"

#include <iostream>

#include "Utility/FileHelper.h"
#include "Utility/StringHelper.h"

#include "GladTypedefs.h"

namespace 
{
	using namespace RenderSystem;

	enum class SHADER_TYPE
	{
		SHADER,
		SHADER_PROGRAM
	};

	std::string getShaderOrProgramInfoLog(GetShaderIV getShaderIVFunc, GetShaderInfoLog getShaderInfoLogFunc, int shaderId)
	{
		int infoLogLength = 0;
		std::string infoLog{ '\0' };

		getShaderIVFunc(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
		infoLog.resize(infoLogLength);
		getShaderInfoLogFunc(shaderId, infoLogLength, &infoLogLength, infoLog.data());

		return infoLog;
	}

	std::string getShaderInfoLog(int shaderId, SHADER_TYPE shaderType)
	{
		auto getShaderIVFunc = shaderType == SHADER_TYPE::SHADER ? glGetShaderiv : glGetProgramiv;
		return getShaderOrProgramInfoLog(getShaderIVFunc, glGetProgramInfoLog, shaderId);
	}

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
    ShaderProgram::ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) :
        mVertexShader(),
        mFragmentShader(),
        mShaderProgram(),
		mModel(),
		mVertexShaderPath(vertexShaderPath),
		mFragmentShaderPath(fragmentShaderPath)
    {
		init();
    }

	ShaderProgram::~ShaderProgram()
	{
		glUseProgram(0);
		glDeleteShader(mVertexShader);
		glDeleteShader(mFragmentShader);
		glDeleteProgram(mShaderProgram);
	}

	void ShaderProgram::init()
	{
		initShaderProgram();
		initUniformLocations();
	}

	void ShaderProgram::initShaderProgram()
	{
		mVertexShader = loadShader(mVertexShaderPath, GL_VERTEX_SHADER);
		mFragmentShader = loadShader(mFragmentShaderPath, GL_FRAGMENT_SHADER);
		mShaderProgram = glCreateProgram();
		glAttachShader(mShaderProgram, mVertexShader);
		glAttachShader(mShaderProgram, mFragmentShader);
		glLinkProgram(mShaderProgram);
		checkShaderOrProgramStatus(glGetProgramiv, mShaderProgram, GL_LINK_STATUS, SHADER_TYPE::SHADER_PROGRAM, "Shader program has not been linked");
		use();
	}

	void ShaderProgram::initUniformLocations()
	{
		mModel = glGetUniformLocation(mShaderProgram, "model");
	}

	void ShaderProgram::setModel(const float* model) const
	{
		glUniformMatrix4fv(mModel, 1, false, model);
	}

	void ShaderProgram::use() const
	{
		glUseProgram(mShaderProgram);
	}
}