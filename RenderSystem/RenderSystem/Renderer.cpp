#include "Renderer.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "Utility/FileHelper.h"

#include "Constants.h"
#include "RenderLogger.h"
#include "Typedefs.h"
#include "RenderPrimitive.h"

namespace
{
	using namespace RenderSystem;

	constexpr Material MAIN_MATERIAL = GOLD_MATERIAL;
	constexpr Material HIGHLIGHT_MATERIAL = RUBY_MATERIAL;
	constexpr Material WIREFRAME_MATERIAL = BLACK_MATERIAL;

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
}

namespace RenderSystem
{
	Renderer::Renderer() :
		mVertexShader(),
		mFragmentShader(),
		mShaderProgram(),
		mRenderWireframe(false),
		mHighlightedFacesIndices(),
		mLighting(),
		mRenderBuffer(),
		mDebugRenderBuffer()
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
		mShaderTransformationSystem.init(mShaderProgram);
		mLighting.init(mShaderProgram);
		mRenderBuffer.init();
		mDebugRenderBuffer.init();
	}

	void Renderer::initShaders()
	{
		mVertexShader = loadShader(R"(../RenderSystem/Shaders/VertexShader.vert)", GL_VERTEX_SHADER);
		mFragmentShader = loadShader(R"(../RenderSystem/Shaders/FragmentShader.frag)", GL_FRAGMENT_SHADER);
		initShaderProgram();
	}

	void Renderer::initShaderProgram()
	{
		mShaderProgram = glCreateProgram();
		glAttachShader(mShaderProgram, mVertexShader);
		glAttachShader(mShaderProgram, mFragmentShader);
		glLinkProgram(mShaderProgram);
		checkShaderOrProgramStatus(glGetProgramiv, mShaderProgram, GL_LINK_STATUS, SHADER_TYPE::SHADER_PROGRAM, "Shader program was not linked");
		glUseProgram(mShaderProgram);
	}

	void Renderer::renderHighlightedFaces()
	{
		renderExtraPrimitives(!mHighlightedFacesIndices.empty(), HIGHLIGHT_MATERIAL, [this]() {
			for (const auto& faceIdx : mHighlightedFacesIndices)
			{
				glDrawArrays(GL_TRIANGLES, faceIdx * 3, 3);
			}
		});
	}

	void Renderer::renderWireframe()
	{
		renderExtraPrimitives(mRenderWireframe, WIREFRAME_MATERIAL, [this]() {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_TRIANGLES, 0, mRenderBuffer.getVertexCount());
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		});
	}

	void Renderer::renderScene()
	{
		glDrawArrays(GL_TRIANGLES, 0, mRenderBuffer.getVertexCount());
	}

	void Renderer::makeMaterialActive(const Material& material)
	{
		mLighting.setMaterial(material);
	}

	void Renderer::invokeDebugRenderAction(const std::function<void()>& action)
	{
		mDebugRenderBuffer.bind();
		action();
		mRenderBuffer.bind();
	}

	void Renderer::renderExtraPrimitives(bool renderCondition, const Material& material, const std::function<void()>& renderFunc)
	{
		if (!renderCondition)
		{
			return;
		}

		makeMaterialActive(material);
		glDepthFunc(GL_LEQUAL);
		renderFunc();
		glDepthFunc(GL_LESS);
		makeMaterialActive(MAIN_MATERIAL);
	}

	void Renderer::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderScene();
		renderHighlightedFaces();
		renderWireframe();
	}

	void Renderer::renderDebug()
	{
		if (!DEBUG_RENDER)
		{
			return;
		}

		invokeDebugRenderAction([this]() {
			int startIndex = 0;
			for (const auto& debugPrimitive : mDebugPrimitives)
			{
				makeMaterialActive(debugPrimitive.material);
				glDrawArrays(debugPrimitive.renderMode, startIndex, debugPrimitive.getVertexCount());
				startIndex += debugPrimitive.getVertexCount();
			}
			makeMaterialActive(MAIN_MATERIAL);
		});
	}

	void Renderer::toggleWireframe()
	{
		mRenderWireframe = !mRenderWireframe;
	}

	void Renderer::setHighlightedFaces(const std::vector<int>& facesIndices)
	{
		mHighlightedFacesIndices = facesIndices;
	}

	void Renderer::addDebugPrimitive(const RenderPrimitive& primitive)
	{
		mDebugPrimitives.push_back(primitive);
		invokeDebugRenderAction([this, &primitive]() {
			mDebugRenderBuffer.appendRenderData(primitive.renderData);
		});
	}

	int Renderer::loadShader(const std::string& shaderPath, int shaderType)
	{
		auto shaderContent = Utility::readFile(shaderPath);
		std::vector<const char*> shaderContentVec {shaderContent.c_str()};
		std::vector<int> shaderContentLengthVec {static_cast<int>(shaderContent.size())};

		auto shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, shaderContentVec.data(), shaderContentLengthVec.data());
		glCompileShader(shader);
		checkShaderOrProgramStatus(glGetShaderiv, shader, GL_COMPILE_STATUS, SHADER_TYPE::SHADER, "Shader was not compiled!");

		return shader;
	}

	ShaderTransformationSystem& Renderer::getShaderTransformationSystem()
	{
		return mShaderTransformationSystem;
	}

	Lighting& Renderer::getLighting()
	{
		return mLighting;
	}

	RenderBuffer& Renderer::getRenderBuffer()
	{
		return mRenderBuffer;
	}

	RenderBuffer& Renderer::getDebugRenderBuffer()
	{
		return mDebugRenderBuffer;
	}
}
