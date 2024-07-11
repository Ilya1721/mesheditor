#pragma once

#include <string>
#include <functional>

#include "GeometryCore/Typedefs.h"
#include "MeshCore/Vertex.h"

#include "Lighting.h"
#include "RenderBuffer.h"
#include "ShaderTransformationSystem.h"
#include "Constants.h"

using namespace GeometryCore;

namespace MeshCore
{
	class Object3D;
}

namespace RenderSystem
{
	struct RenderPrimitive;

	class Renderer
	{
	public:
		Renderer();
		Renderer(Renderer&&) = delete;
		~Renderer();

		ShaderTransformationSystem& getShaderTransformationSystem();
		Lighting& getLighting();
		RenderBuffer& getRenderBuffer();

		void init(const MeshCore::Object3D* sceneRootObject);
		void render();
		void toggleWireframe();
		void setHighlightedFaces(const std::vector<int>& facesIndices);

		void renderVerticesNormals(const std::vector<MeshCore::Vertex>& vertices);
		void renderLine(const Point3D& startPos, const Point3D& endPos, const Material& material = GREEN_MATERIAL, bool withArrow = false);

	private:
		void initShaders();
		void initShaderProgram();
		void invokeExtraRenderAction(const std::function<void()>& action, bool loadBuffer = false);

		void addInitialExtraPrimitives();
		void addSceneFloor();
		void addExtraPrimitive(const RenderPrimitive& primitive);
		void addLineExtraPrimitive(const Point3D& start, const Point3D& end, const Material& material);
		void addPlaneExtraPrimitive(const Point3D& origin, const Vector3D& normal, float width, float height, const Material& material);

		void addGlobalAxes(float length = 10.0f);
		void renderExtra();
		void renderHighlightedFaces();
		void renderWireframe();
		void renderScene();
		void renderOverlayPrimitives(bool renderCondition, const Material& material, const std::function<void()>& renderFunc);

	private:
		int mVertexShader;
		int mFragmentShader;
		int mShaderProgram;

		bool mRenderWireframe;
		std::vector<int> mHighlightedFacesIndices;

		ShaderTransformationSystem mShaderTransformationSystem;
		Lighting mLighting;
		RenderBuffer mRenderBuffer;

		std::vector<RenderPrimitive> mExtraPrimitives;
		RenderBuffer mExtraRenderBuffer;

		const MeshCore::Object3D* mSceneRootObject;
	};
}

