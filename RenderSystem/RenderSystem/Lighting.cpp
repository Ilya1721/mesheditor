#include "Lighting.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include "Constants.h"

namespace RenderSystem
{
	Lighting::Lighting() :
		mLightPos(0),
		mShaderProgram(0),
		mObjectColor(0),
		mLightColor(0),
		mAmbientStrength(0),
		mSpecularStrength(0),
		mShininess(0),
		mCameraPos(0)
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
		mLightColor = glGetUniformLocation(mShaderProgram, "lightColor");
		mAmbientStrength = glGetUniformLocation(mShaderProgram, "ambientStrength");
		mSpecularStrength = glGetUniformLocation(mShaderProgram, "specularStrength");
		mShininess = glGetUniformLocation(mShaderProgram, "shininess");
		mCameraPos = glGetUniformLocation(mShaderProgram, "cameraPos");
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

	void Lighting::setLightColor(const float* color)
	{
		glUniform3fv(mLightColor, 1, color);
	}

	void Lighting::setCameraPos(const float* pos)
	{
		glUniform3fv(mCameraPos, 1, pos);
	}

	void Lighting::setAmbientStrength(float strength)
	{
		glUniform1f(mAmbientStrength, strength);
	}

	void Lighting::setSpecularStrength(float strength)
	{
		glUniform1f(mSpecularStrength, strength);
	}

	void Lighting::setShininess(int shininess)
	{
		glUniform1i(mShininess, shininess);
	}
}
