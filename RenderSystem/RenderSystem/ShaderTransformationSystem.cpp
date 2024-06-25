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
		mViewModel = glGetUniformLocation(mShaderProgram, "viewModel");
		mProjection = glGetUniformLocation(mShaderProgram, "projection");
	}

	void ShaderTransformationSystem::setViewModel(const float* view) const
	{
		glUniformMatrix4fv(mViewModel, 1, false, view);
	}

	void ShaderTransformationSystem::setProjection(const float* projection) const
	{
		glUniformMatrix4fv(mProjection, 1, false, projection);
	}
}