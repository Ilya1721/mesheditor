#pragma once

#include "MeshCore/RenderData.h"

namespace RenderSystem
{
	class RenderBuffer
	{
	public:
		RenderBuffer();

		void init(int shaderProgram);

		void setRenderData(const MeshCore::RenderData& renderData);
		void setModel(const float* model);
		void setView(const float* view);
		void setProjection(const float* projection);

		int getVertexCount() const;

	private:
		void initUniformLocations();
		void initBuffers();

	private:
		int mShaderProgram;

		int mModel;
		int mView;
		int mProjection;

		unsigned int mVBO;
		unsigned int mVAO;
		int mVertexCount;
	};
}

