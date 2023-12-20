#include "ShaderTransformationSystem.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

namespace RenderSystem
{
	void ShaderTransformationSystem::init(int shaderProgram)
	{
		mShaderProgram = shaderProgram;
		initUniformLocations();
	}

	void ShaderTransformationSystem::initUniformLocations()
	{
		mModel = glGetUniformLocation(mShaderProgram, "model");
		mView = glGetUniformLocation(mShaderProgram, "view");
		mProjection = glGetUniformLocation(mShaderProgram, "projection");
	}

	void ShaderTransformationSystem::setModel(const float* model)
	{
		glUniformMatrix4fv(mModel, 1, false, model);
	}

	void ShaderTransformationSystem::setView(const float* view)
	{
		glUniformMatrix4fv(mView, 1, false, view);
	}

	void ShaderTransformationSystem::setProjection(const float* projection)
	{
		glUniformMatrix4fv(mProjection, 1, false, projection);
	}
}