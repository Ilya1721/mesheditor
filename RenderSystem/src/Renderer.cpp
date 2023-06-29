module;
#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"
module Renderer;

import <iostream>;

import FileHelper;
import RenderLogger;

namespace RenderSystem
{
	Renderer::Renderer() :
		mVertexShader(0),
		mFragmentShader(0),
		mShaderProgram(0),
		mLighting(),
		mRenderBuffer()
	{
		init();
	}

	Renderer::~Renderer()
	{
		glUseProgram(0);
		glDeleteShader(mVertexShader);
		glDeleteShader(mFragmentShader);
		glDeleteProgram(mShaderProgram);
	}

	void Renderer::init()
	{
		initShaders();
		mLighting.init(mShaderProgram);
		mRenderBuffer.init(mShaderProgram);
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

		glUseProgram(mShaderProgram);
	}

	void Renderer::render() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, mRenderBuffer.getVertexCount());
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

	Lighting& Renderer::getLighting()
	{
		return mLighting;
	}

	RenderBuffer& Renderer::getRenderBuffer()
	{
		return mRenderBuffer;
	}
}
