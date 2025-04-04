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

	void RenderBuffer::bind(unsigned int vbo, unsigned int vao) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindVertexArray(vao);
	}

	void RenderBuffer::invokeAction(const std::function<void()>& action) const
	{
		GLint vaoToRestore = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vaoToRestore);
		GLint vboToRestore = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vboToRestore);
		bind(mVBO, mVAO);
		action();
		bind(vaoToRestore, vboToRestore);
	}

	void RenderBuffer::loadRenderData(const RenderData& renderData, int attributesPerVertex)
	{
		invokeAction([&renderData, attributesPerVertex]() {
			const auto& compactData = renderData.getCompactData();
			glBufferData(GL_ARRAY_BUFFER, compactData.size() * sizeof(float), compactData.data(), GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, attributesPerVertex, GL_FLOAT, GL_FALSE, attributesPerVertex * 2 * sizeof(float), 0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(
				1, attributesPerVertex, GL_FLOAT, GL_FALSE, attributesPerVertex * 2 * sizeof(float),
				reinterpret_cast<void*>(attributesPerVertex * sizeof(float))
			);
		});
	}
}
