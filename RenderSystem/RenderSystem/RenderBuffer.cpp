#include "RenderBuffer.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include "MeshCore/Constants.h"

namespace RenderSystem
{
	void RenderBuffer::init()
	{
		glGenBuffers(1, &mVBO);
		glGenVertexArrays(1, &mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBindVertexArray(mVAO);
	}

	void RenderBuffer::setRenderData(const MeshCore::RenderData& renderData)
	{
		mVertexCount = renderData.positions.size() / MeshCore::COORDINATES_PER_VERTEX;
		auto compactData = renderData.getCompactData();

		glBufferData(GL_ARRAY_BUFFER, compactData.size() * sizeof(float), compactData.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(renderData.normals.size() * sizeof(float)));
	}

	int RenderBuffer::getVertexCount() const
	{
		return mVertexCount;
	}
}
