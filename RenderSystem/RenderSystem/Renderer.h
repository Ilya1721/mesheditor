#pragma once

#include <string>

#include "Lighting.h"
#include "RenderBuffer.h"

namespace RenderSystem
{
	class Renderer final
	{
	public:
		Renderer();
		~Renderer();

		void render() const;

		Lighting& getLighting();
		RenderBuffer& getRenderBuffer();

	private:
		int loadShader(const std::string& filePath, int shaderType);
		void init();
		void initShaders();

	private:
		int mVertexShader;
		int mFragmentShader;
		int mShaderProgram;

		Lighting mLighting;
		RenderBuffer mRenderBuffer;
	};
}

