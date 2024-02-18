#pragma once

#include <string>

#include "Lighting.h"
#include "RenderBuffer.h"
#include "ShaderTransformationSystem.h"

namespace RenderSystem
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void render();
		void setHighlightedFaces(const std::vector<int>& facesIndices);

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
		void makeMaterialForSceneActive();
		void makeMaterialForHighlightActive();

	private:
		int mVertexShader;
		int mFragmentShader;
		int mShaderProgram;
		std::vector<int> mHighlightedFacesIndices;

		ShaderTransformationSystem mShaderTransformationSystem;
		Lighting mLighting;
		RenderBuffer mRenderBuffer;
	};
}

