#include "RenderBuffer.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include "MeshCore/Constants.h"

namespace RenderSystem
{
	RenderBuffer::RenderBuffer()
	{
		init();
	}

	RenderBuffer::~RenderBuffer()
	{
		glDeleteBuffers(1, &mVBO);
		glDeleteVertexArrays(1, &mVAO);
	}

	void RenderBuffer::init()
	{
		glGenBuffers(1, &mVBO);
		glGenVertexArrays(1, &mVAO);
	}

	void RenderBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBindVertexArray(mVAO);
	}

	void RenderBuffer::loadRenderData(const MeshCore::RenderData& renderData)
	{
		const auto& compactData = renderData.getCompactData();
		glBufferData(GL_ARRAY_BUFFER, compactData.size() * sizeof(float), compactData.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	}
}
