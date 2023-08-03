#include "RenderBuffer.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include "MeshCore/Constants.h"

namespace RenderSystem
{
	RenderBuffer::RenderBuffer() :
		mShaderProgram(0),
		mModel(0),
		mView(0),
		mProjection(0),
		mVBO(0),
		mVAO(0),
		mVertexCount(0)
	{}

	void RenderBuffer::init(int shaderProgram)
	{
		mShaderProgram = shaderProgram;
		initUniformLocations();
		initBuffers();
	}

	void RenderBuffer::initUniformLocations()
	{
		mModel = glGetUniformLocation(mShaderProgram, "model");
		mView = glGetUniformLocation(mShaderProgram, "view");
		mProjection = glGetUniformLocation(mShaderProgram, "projection");
	}

	void RenderBuffer::initBuffers()
	{
		glGenBuffers(1, &mVBO);
		glGenVertexArrays(1, &mVAO);
	}

	void RenderBuffer::setRenderData(const MeshCore::RenderData& renderData)
	{
		mVertexCount = static_cast<int>(renderData.positions.size()) / MeshCore::COORDINATES_PER_VERTEX;

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBindVertexArray(mVAO);
		auto compactData = renderData.getCompactData();
		glBufferData(GL_ARRAY_BUFFER, compactData.size() * sizeof(float), compactData.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(renderData.positions.size() * sizeof(float)));
	}

	void RenderBuffer::setModel(const float* model)
	{
		glUniformMatrix4fv(mModel, 1, false, model);
	}

	void RenderBuffer::setView(const float* view)
	{
		glUniformMatrix4fv(mView, 1, false, view);
	}

	void RenderBuffer::setProjection(const float* projection)
	{
		glUniformMatrix4fv(mProjection, 1, false, projection);
	}

	int RenderBuffer::getVertexCount() const
	{
		return mVertexCount;
	}
}
