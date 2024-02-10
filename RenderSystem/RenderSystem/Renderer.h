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
		void setHighlightedFaceIdx(int faceIdx);

		ShaderTransformationSystem& getShaderTransformationSystem();
		Lighting& getLighting();
		RenderBuffer& getRenderBuffer();

	private:
		int loadShader(const std::string& filePath, int shaderType);
		void init();
		void initShaders();
		void initShaderProgram();
		void renderHighlightedFace();
		void renderScene();
		void makeMaterialForSceneActive();
		void makeMaterialForHighlightActive();

	private:
		int mVertexShader;
		int mFragmentShader;
		int mShaderProgram;
		int mHighlightedFaceIdx;

		ShaderTransformationSystem mShaderTransformationSystem;
		Lighting mLighting;
		RenderBuffer mRenderBuffer;
	};
}

