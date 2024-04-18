#pragma once

#include <string>
#include <functional>

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
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = default;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = default;
		~Renderer();

		void render();
		void renderDebug();
		void setHighlightedFaces(const std::vector<int>& facesIndices);
		void addDebugPrimitive(const RenderPrimitive& primitive);

		ShaderTransformationSystem& getShaderTransformationSystem();
		Lighting& getLighting();
		RenderBuffer& getRenderBuffer();

	private:
		int loadShader(const std::string& filePath, int shaderType);
		void init();
		void initShaders();
		void initShaderProgram();
		void renderHighlightedFaces();
		void renderScene();
		void makeMaterialActive(const Material& material);
		void invokeDebugRenderAction(const std::function<void()>& action);

	private:
		int mVertexShader;
		int mFragmentShader;
		int mShaderProgram;
		std::vector<int> mHighlightedFacesIndices;

		ShaderTransformationSystem mShaderTransformationSystem;
		Lighting mLighting;
		RenderBuffer mRenderBuffer;

		std::vector<RenderPrimitive> mDebugPrimitives;
		RenderBuffer mDebugRenderBuffer;
	};
}

