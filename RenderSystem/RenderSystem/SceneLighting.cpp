#include "SceneLighting.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"
#include "Material.h"
#include "Light.h"

namespace RenderSystem
{
	void SceneLighting::init(int shaderProgram)
	{
		mShaderProgram = shaderProgram;
		initUniformLocations();
		setupSettings();
	}

	void SceneLighting::setupSettings()
	{
		glClearColor(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
		glEnable(GL_DEPTH_TEST);
		setLight(DEFAULT_LIGHT);
		setMaterial(GOLD_MATERIAL);
	}

	int SceneLighting::getUniformLocation(const char* name) const
	{
		return glGetUniformLocation(mShaderProgram, name);
	}

	void SceneLighting::initUniformLocations()
	{
		mLightPos = getUniformLocation("lightPosInCameraSpace");
		mCameraPos = getUniformLocation("cameraPosInCameraSpace");
		mLightAmbient = getUniformLocation("light.ambient");
		mLightDiffuse = getUniformLocation("light.diffuse");
		mLightSpecular = getUniformLocation("light.specular");
		mMaterialAmbient = getUniformLocation("material.ambient");
		mMaterialDiffuse = getUniformLocation("material.diffuse");
		mMaterialSpecular = getUniformLocation("material.specular");
		mMaterialShininess = getUniformLocation("material.shininess");
	}

	void SceneLighting::setMaterial(const Material& material)
	{
		setMaterialAmbient(glm::value_ptr(material.ambient));
		setMaterialDiffuse(glm::value_ptr(material.diffuse));
		setMaterialSpecular(glm::value_ptr(material.specular));
		setMaterialShininess(material.shininess);
	}

	void SceneLighting::setLight(const Light& light)
	{
		setLightAmbient(glm::value_ptr(light.ambient));
		setLightDiffuse(glm::value_ptr(light.diffuse));
		setLightSpecular(glm::value_ptr(light.specular));
	}

	void SceneLighting::setLightPos(const float* lightPos) const
	{
		glUniform3fv(mLightPos, 1, lightPos);
	}

	void SceneLighting::setCameraPos(const float* pos) const
	{
		glUniform3fv(mCameraPos, 1, pos);
	}

	void SceneLighting::setLightAmbient(const float* ambient) const
	{
		glUniform3fv(mLightAmbient, 1, ambient);
	}

	void SceneLighting::setLightDiffuse(const float* diffuse) const
	{
		glUniform3fv(mLightDiffuse, 1, diffuse);
	}

	void SceneLighting::setLightSpecular(const float* specular) const
	{
		glUniform3fv(mLightSpecular, 1, specular);
	}

	void SceneLighting::setMaterialAmbient(const float* ambient) const
	{
		glUniform3fv(mMaterialAmbient, 1, ambient);
	}

	void SceneLighting::setMaterialDiffuse(const float* diffuse) const
	{
		glUniform3fv(mMaterialDiffuse, 1, diffuse);
	}

	void SceneLighting::setMaterialSpecular(const float* specular) const
	{
		glUniform3fv(mMaterialSpecular, 1, specular);
	}

	void SceneLighting::setMaterialShininess(float shininess) const
	{
		glUniform1f(mMaterialShininess, shininess);
	}
}
