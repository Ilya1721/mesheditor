#pragma once

#include "MeshCore/RenderData.h"

namespace RenderSystem
{
	class RenderBuffer
	{
	public:
		RenderBuffer() = default;

		void init();
		void bind();
		void appendRenderData(const MeshCore::RenderData& renderData);
		void setRenderData(const MeshCore::RenderData& renderData);
		int getVertexCount() const;

	private:
		void load();

	private:
		unsigned int mVBO;
		unsigned int mVAO;
		int mVertexCount;
		MeshCore::RenderData mRenderData;
	};
}

