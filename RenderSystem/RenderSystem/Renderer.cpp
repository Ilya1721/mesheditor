#include "Renderer.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "Utility/FileHelper.h"
#include "Utility/StringHelper.h"
#include "GeometryCore/Line.h"
#include "GeometryCore/Plane.h"
#include "MeshCore/Object3D.h"

#include "RenderLogger.h"
#include "GladTypedefs.h"
#include "RenderPrimitive.h"

namespace
{
	using namespace RenderSystem;

	constexpr Material MAIN_MATERIAL = GOLD_MATERIAL;
	constexpr Material HIGHLIGHT_MATERIAL = RUBY_MATERIAL;
	constexpr Material WIREFRAME_MATERIAL = BLACK_MATERIAL;
	constexpr Material FLOOR_MATERIAL = PEARL_MATERIAL;

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
		mExtraRenderBuffer(),
		mShaderTransformationSystem(),
		mSceneRootObject(nullptr)
	{}

	Renderer::~Renderer()
	{
		glUseProgram(0);
		glDeleteShader(mVertexShader);
		glDeleteShader(mFragmentShader);
		glDeleteProgram(mShaderProgram);
	}

	void Renderer::init(const MeshCore::Object3D* sceneRootObject)
	{
		mSceneRootObject = sceneRootObject;
		initShaders();
		mShaderTransformationSystem.init(mShaderProgram);
		mShaderTransformationSystem.setModel(glm::value_ptr(mSceneRootObject->getTransform()));
		mLighting.init(mShaderProgram);
		addInitialExtraPrimitives();
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

	void Renderer::addInitialExtraPrimitives()
	{
		invokeExtraRenderAction([this]() {
			addSceneFloor();
		}, true);
	}

	void Renderer::addSceneFloor()
	{
		auto originY = -mSceneRootObject->getBBox().getHeight() * FLOOR_BBOX_HEIGHT_COEF;
		addPlaneExtraPrimitive(Point3D(0.0f, originY, 0.0f), Vector3D(0.0f, -1.0f, 0.0f), FAR_PLANE_DISTANCE, FAR_PLANE_DISTANCE, FLOOR_MATERIAL);
	}

	void Renderer::renderHighlightedFaces()
	{
		renderOverlayPrimitives(!mHighlightedFacesIndices.empty(), HIGHLIGHT_MATERIAL, [this]() {
			for (const auto& faceIdx : mHighlightedFacesIndices)
			{
				glDrawArrays(GL_TRIANGLES, faceIdx * 3, 3);
			}
		});
	}

	void Renderer::renderWireframe()
	{
		renderOverlayPrimitives(mRenderWireframe, WIREFRAME_MATERIAL, [this]() {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_TRIANGLES, 0, mRenderBuffer.getRenderData().getVertexCount());
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		});
	}

	void Renderer::renderScene()
	{
		glDrawArrays(GL_TRIANGLES, 0, mRenderBuffer.getRenderData().getVertexCount());
	}

	void Renderer::invokeExtraRenderAction(const std::function<void()>& action, bool loadBuffer)
	{
		mExtraRenderBuffer.bind();
		mShaderTransformationSystem.setModel(glm::value_ptr(glm::mat4(1.0f)));
		action();
		if (loadBuffer)
		{
			mExtraRenderBuffer.load();
		}
		mShaderTransformationSystem.setModel(glm::value_ptr(mSceneRootObject->getTransform()));
		mRenderBuffer.bind();
	}

	void Renderer::renderOverlayPrimitives(bool renderCondition, const Material& material, const std::function<void()>& renderFunc)
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
		renderExtra();
	}

	void Renderer::renderExtra()
	{
		invokeExtraRenderAction([this]() {
			int startIndex = 0;
			for (const auto& extraPrimitive : mExtraPrimitives)
			{
				auto vertexCount = extraPrimitive.renderData.getVertexCount();
				mLighting.setMaterial(extraPrimitive.material);
				glDrawArrays(extraPrimitive.renderMode, startIndex, vertexCount);
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

	void Renderer::addExtraPrimitive(const RenderPrimitive& primitive)
	{
		mExtraPrimitives.push_back(primitive);
		mExtraRenderBuffer.appendRenderData(primitive.renderData);
	}

	void Renderer::addLineExtraPrimitive(const Point3D& start, const Point3D& end, const Material& material)
	{
		Line line{ start, end };
		auto linePrimitive = RenderPrimitive::createPrimitive(line, true, material);
		addExtraPrimitive(linePrimitive);
	}

	void Renderer::addPlaneExtraPrimitive(const Point3D& origin, const Vector3D& normal, float width, float height, const Material& material)
	{
		Plane plane{ origin, normal };
		auto planePrimitive = RenderPrimitive::createPrimitive(plane, width, height, material);
		addExtraPrimitive(planePrimitive);
	}

	void Renderer::renderAxes(float length)
	{
		invokeExtraRenderAction([this, &length]() {
			addLineExtraPrimitive(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(length, 0.0f, 0.0f), BLUE_MATERIAL);
			addLineExtraPrimitive(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, length, 0.0f), RED_MATERIAL);
			addLineExtraPrimitive(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, length), GREEN_MATERIAL);
		}, true);
	}

	void Renderer::renderVerticesNormals(const std::vector<MeshCore::Vertex>& vertices)
	{
		invokeExtraRenderAction([this, &vertices]() {
			for (const auto& vertex : vertices)
			{
				addLineExtraPrimitive(vertex.pos, vertex.pos + vertex.normal * 10.0f, GREEN_MATERIAL);
			}
		}, true);
	}

	void Renderer::renderLine(const Point3D& startPos, const Point3D& endPos, const Material& material, bool withArrow)
	{
		invokeExtraRenderAction([this, &startPos, &endPos, &material, &withArrow]() {
			Line line{ startPos, endPos };
			auto linePrimitive = RenderPrimitive::createPrimitive(line, withArrow, material);
			addExtraPrimitive(linePrimitive);
		}, true);
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
