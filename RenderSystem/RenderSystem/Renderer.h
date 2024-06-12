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

		void render();
		void toggleWireframe();
		void setHighlightedFaces(const std::vector<int>& facesIndices);
		void renderAxes();
		void renderVerticesNormals(const std::vector<MeshCore::Vertex>& vertices);
		void renderVectorOnVertex(const Point3D& vertexPos, const Vector3D& vector);
		void renderPlaneOnVertex(const Point3D& vertexPos, const Vector3D& planeNormal);
		void renderLine(const Point3D& startPos, const Point3D& endPos, const Material& material = GREEN_MATERIAL, bool withArrow = false);
		void clearDebugRenderBuffer();

	private:
		void renderDebug();
		void init();
		void initShaders();
		void initShaderProgram();
		void renderHighlightedFaces();
		void renderWireframe();
		void renderScene();
		void invokeDebugRenderAction(const std::function<void()>& action, bool loadBuffer = false);
		void renderExtraPrimitives(bool renderCondition, const Material& material, const std::function<void()>& renderFunc);
		void addDebugPrimitive(const RenderPrimitive& primitive);
		void addLineDebugPrimitive(const Point3D& start, const Point3D& end, const Material& material);

	private:
		int mVertexShader;
		int mFragmentShader;
		int mShaderProgram;

		bool mRenderWireframe;
		std::vector<int> mHighlightedFacesIndices;

		ShaderTransformationSystem mShaderTransformationSystem;
		Lighting mLighting;
		RenderBuffer mRenderBuffer;

		std::vector<RenderPrimitive> mDebugPrimitives;
		RenderBuffer mDebugRenderBuffer;
	};
}

