#pragma once

#include "MeshCore/RenderData.h"

namespace RenderSystem
{
	class RenderBuffer
	{
	public:
		RenderBuffer() = default;

		void init();
		void setRenderData(const MeshCore::RenderData& renderData);
		int getVertexCount() const;

	private:
		unsigned int mVBO;
		unsigned int mVAO;
		int mVertexCount;
	};
}

