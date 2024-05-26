#pragma once

#include <string>
#include <functional>

#include "MeshCore/Vertex.h"

#include "Lighting.h"
#include "RenderBuffer.h"
#include "ShaderTransformationSystem.h"

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

