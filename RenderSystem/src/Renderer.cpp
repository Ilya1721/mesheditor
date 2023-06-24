module;
#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"
module Renderer;

import <iostream>;

import FileHelper;
import RenderSystemConsts;

namespace RenderSystem
{
	Renderer::Renderer() :
		mVertexShader(0),
		mFragmentShader(0),
		mShaderProgram(0),
		mModel(0),
		mView(0),
		mProjection(0),
		mVBO(0),
		mVAO(0)
	{
		init();
	}

	Renderer::~Renderer() noexcept
	{
		glUseProgram(0);
		glDeleteShader(mVertexShader);
		glDeleteShader(mFragmentShader);
		glDeleteProgram(mShaderProgram);
	}

	void Renderer::init()
	{
		initShaders();

		glClearColor(static_cast<float>(BACKGROUND_COLOR.r()), static_cast<float>(BACKGROUND_COLOR.g()),
			static_cast<float>(BACKGROUND_COLOR.b()), static_cast<float>(BACKGROUND_COLOR.a()));
		glEnable(GL_DEPTH_TEST);

		glGenBuffers(1, &mVBO);
		glGenVertexArrays(1, &mVAO);
	}

	void Renderer::initShaders()
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
			const auto& shaderLog = getLog(mShaderProgram, SHADER_LOG_TYPE::SHADER_PROGRAM);
			std::cerr << "Shader program was not linked\n" << shaderLog.data() << std::endl;
			throw std::exception("Shader program was not linked");
		}

		mModel = glGetUniformLocation(mShaderProgram, "model");
		mView = glGetUniformLocation(mShaderProgram, "view");
		mProjection = glGetUniformLocation(mShaderProgram, "projection");

		glUseProgram(mShaderProgram);
	}

	void Renderer::setRenderData(const MeshCore::RenderData& renderData) noexcept
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBindVertexArray(mVAO);
		auto compactData = renderData.getCompactData();
		glBufferData(GL_ARRAY_BUFFER, compactData.size() * sizeof(float), compactData.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(renderData.positions.size() * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0,
			reinterpret_cast<void*>((renderData.positions.size() + renderData.normals.size()) * sizeof(float)));
	}

	void Renderer::setModel(const float* model) noexcept
	{
		glUniformMatrix4fv(mModel, 1, false, model);
	}

	void Renderer::setView(const float* view) noexcept
	{
		glUniformMatrix4fv(mView, 1, false, view);
	}

	void Renderer::setProjection(const float* projection) noexcept
	{
		glUniformMatrix4fv(mProjection, 1, false, projection);

	}

	void Renderer::render() const noexcept
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	int Renderer::loadShader(const std::string& shaderPath, int shaderType)
	{
		auto shader = glCreateShader(shaderType);
		auto shaderStr = Helpers::readFile(shaderPath);
		std::vector<const char*> shaderStrVec {shaderStr.c_str()};
		std::vector<int> shaderStrLengthsVec {static_cast<int>(shaderStr.size())};

		glShaderSource(shader, 1, shaderStrVec.data(), shaderStrLengthsVec.data());
		glCompileShader(shader);

		int isCompiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (!isCompiled)
		{
			auto shaderLog = getLog(shader, SHADER_LOG_TYPE::SHADER);
			std::cerr << "Shader was not compiled!\n" << shaderLog.data() << std::endl;
			throw std::exception("Shader was not compiled!");
		}

		return shader;
	}

	std::string Renderer::getLog(int shaderOrProgramId, SHADER_LOG_TYPE logType) noexcept
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

	void Renderer::printOpenGLErrorMessage() noexcept
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
