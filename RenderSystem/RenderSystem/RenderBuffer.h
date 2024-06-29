#pragma once

#include "MeshCore/RenderData.h"

namespace RenderSystem
{
	class RenderBuffer
	{
	public:
		RenderBuffer();
		RenderBuffer(RenderBuffer&&) = delete;
		~RenderBuffer();

		const MeshCore::RenderData& getRenderData() const;

		void bind() const;
		void load();
		void appendRenderData(const MeshCore::RenderData& renderData);
		void setRenderData(const MeshCore::RenderData& renderData);

	private:
		void init();

	private:
		unsigned int mVBO;
		unsigned int mVAO;
		int mVertexCount;
		MeshCore::RenderData mRenderData;
	};
}

