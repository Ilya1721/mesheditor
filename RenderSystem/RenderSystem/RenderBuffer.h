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

		void bind() const;
		void loadRenderData(const MeshCore::RenderData& renderData);

	private:
		void init();

	private:
		unsigned int mVBO;
		unsigned int mVAO;
	};
}

