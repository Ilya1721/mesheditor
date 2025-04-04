#pragma once

#include <functional>

#include "RenderData.h"

namespace RenderSystem
{
	class RenderBuffer
	{
	public:
		RenderBuffer();
		RenderBuffer(RenderBuffer&&) = delete;
		~RenderBuffer();

		void invokeAction(const std::function<void()>& action) const;
		void loadRenderData(const RenderData& renderData, int attributesPerVertex = 3);

	private:
		void bind(unsigned int vbo, unsigned int vao) const;
		void init();

	private:
		unsigned int mVBO;
		unsigned int mVAO;
	};
}

