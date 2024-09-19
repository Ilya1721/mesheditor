#pragma once

#include "MeshCore/RenderData.h"

namespace RenderSystem
{
	class RenderBuffer
	{
	public:
		RenderBuffer() = default;
		RenderBuffer(RenderBuffer&&) = delete;
		~RenderBuffer();

		void bind() const;
		void init();
		void loadRenderData(const MeshCore::RenderData& renderData);

	private:
		unsigned int mVBO;
		unsigned int mVAO;
	};
}

