module;
#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"
module Lighting;

import RenderSystemConsts;

namespace RenderSystem
{
	Lighting::Lighting() :
		mLightPos(0),
		mShaderProgram(0),
		mObjectColor(0),
		mAmbientColor(0),
		mAmbientStrength(0),
		mDiffuseColor(0)
	{}

	void Lighting::init(int shaderProgram)
	{
		mShaderProgram = shaderProgram;
		initUniformLocations();
		setUpSettings();
	}

	void Lighting::initUniformLocations()
	{
		mLightPos = glGetUniformLocation(mShaderProgram, "lightPos");
		mObjectColor = glGetUniformLocation(mShaderProgram, "objectColor");
		mAmbientColor = glGetUniformLocation(mShaderProgram, "ambientColor");
		mAmbientStrength = glGetUniformLocation(mShaderProgram, "ambientStrength");
		mDiffuseColor = glGetUniformLocation(mShaderProgram, "diffuseColor");
	}

	void Lighting::setUpSettings()
	{
		glClearColor(static_cast<float>(BACKGROUND_COLOR.r()), static_cast<float>(BACKGROUND_COLOR.g()),
			static_cast<float>(BACKGROUND_COLOR.b()), static_cast<float>(BACKGROUND_COLOR.a()));
		glEnable(GL_DEPTH_TEST);
	}

	void Lighting::setLightPos(const float* lightPos)
	{
		glUniform3fv(mLightPos, 1, lightPos);
	}

	void Lighting::setObjectColor(const float* color)
	{
		glUniform3fv(mObjectColor, 1, color);
	}

	void Lighting::setAmbientColor(const float* color)
	{
		glUniform3fv(mAmbientColor, 1, color);
	}

	void Lighting::setDiffuseColor(const float* color)
	{
		glUniform3fv(mDiffuseColor, 1, color);
	}

	void Lighting::setAmbientStrength(float strength)
	{
		glUniform1f(mAmbientStrength, strength);
	}
}
