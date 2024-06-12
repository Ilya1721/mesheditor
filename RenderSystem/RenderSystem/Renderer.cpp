#include "Renderer.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "Utility/FileHelper.h"
#include "Utility/StringHelper.h"
#include "GeometryCore/Line.h"
#include "GeometryCore/Plane.h"

#include "RenderLogger.h"
#include "GladTypedefs.h"
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
		mRenderBuffer.bind();
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
			glDrawArrays(GL_TRIANGLES, 0, mRenderBuffer.getRenderData().getVertexCount());
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		});
	}

	void Renderer::renderScene()
	{
		glDrawArrays(GL_TRIANGLES, 0, mRenderBuffer.getRenderData().getVertexCount());
	}

	void Renderer::invokeDebugRenderAction(const std::function<void()>& action, bool loadBuffer)
	{
		mDebugRenderBuffer.bind();
		action();
		if (loadBuffer)
		{
			mDebugRenderBuffer.load();
		}
		mRenderBuffer.bind();
	}

	void Renderer::renderExtraPrimitives(bool renderCondition, const Material& material, const std::function<void()>& renderFunc)
	{
		if (!renderCondition)
		{
			return;
		}

		mLighting.setMaterial(material);
		glDepthFunc(GL_LEQUAL);
		renderFunc();
		glDepthFunc(GL_LESS);
		mLighting.setMaterial(MAIN_MATERIAL);
	}

	void Renderer::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderScene();
		renderHighlightedFaces();
		renderWireframe();

		if (DEBUG_RENDER)
		{
			renderDebug();
		}
	}

	void Renderer::renderDebug()
	{
		invokeDebugRenderAction([this]() {
			int startIndex = 0;
			for (const auto& debugPrimitive : mDebugPrimitives)
			{
				auto vertexCount = debugPrimitive.renderData.getVertexCount();
				mLighting.setMaterial(debugPrimitive.material);
				glDrawArrays(debugPrimitive.renderMode, startIndex, vertexCount);
				startIndex += vertexCount;
			}
			mLighting.setMaterial(MAIN_MATERIAL);
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
		mDebugRenderBuffer.appendRenderData(primitive.renderData);
	}

	void Renderer::addLineDebugPrimitive(const Point3D& start, const Point3D& end, const Material& material)
	{
		Line line{ start, end };
		auto linePrimitive = RenderPrimitive::createPrimitive(line, true, material);
		addDebugPrimitive(linePrimitive);
	}

	void Renderer::renderAxes()
	{
		invokeDebugRenderAction([this]() {
			addLineDebugPrimitive(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 0.0f, 0.0f), BLUE_MATERIAL);
			addLineDebugPrimitive(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 10.0f, 0.0f), RED_MATERIAL);
			addLineDebugPrimitive(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 10.0f), GREEN_MATERIAL);
		}, true);
	}

	void Renderer::renderVerticesNormals(const std::vector<MeshCore::Vertex>& vertices)
	{
		invokeDebugRenderAction([this, &vertices]() {
			for (const auto& vertex : vertices)
			{
				addLineDebugPrimitive(vertex.pos, vertex.pos + vertex.normal * 10.0f, GREEN_MATERIAL);
			}
		}, true);
	}

	void Renderer::renderVectorOnVertex(const Point3D& vertexPos, const Vector3D& vector)
	{
		invokeDebugRenderAction([this, &vertexPos, &vector]() {
			addLineDebugPrimitive(vertexPos, vertexPos + vector * 10.0f, GREEN_MATERIAL);
		}, true);
	}

	void Renderer::renderPlaneOnVertex(const Point3D& vertexPos, const Vector3D& planeNormal)
	{
		invokeDebugRenderAction([this, &vertexPos, &planeNormal]() {
			Plane plane{ vertexPos, planeNormal };
			auto planePrimitive = RenderPrimitive::createPrimitive(plane, 50.0f, 50.0f, GREEN_MATERIAL);
			addDebugPrimitive(planePrimitive);
		}, true);
	}

	void Renderer::renderLine(const Point3D& startPos, const Point3D& endPos, const Material& material, bool withArrow)
	{
		invokeDebugRenderAction([this, &startPos, &endPos, &material, &withArrow]() {
			Line line{ startPos, endPos };
			auto linePrimitive = RenderPrimitive::createPrimitive(line, withArrow, material);
			addDebugPrimitive(linePrimitive);
		}, true);
	}

	void Renderer::clearDebugRenderBuffer()
	{
		mDebugPrimitives.clear();
		mDebugRenderBuffer.setRenderData({});
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
}
