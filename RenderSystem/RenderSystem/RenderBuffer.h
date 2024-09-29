#pragma once

#include "RenderData.h"

namespace RenderSystem
{
	class RenderBuffer
	{
	public:
		RenderBuffer();
		RenderBuffer(RenderBuffer&&) = delete;
		~RenderBuffer();

		void bind() const;
		void loadRenderData(const RenderData& renderData);

	private:
		void init();

	private:
		unsigned int mVBO;
		unsigned int mVAO;
	};
}

