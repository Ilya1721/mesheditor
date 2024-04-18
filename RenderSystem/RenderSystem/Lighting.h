#pragma once

namespace RenderSystem
{
	struct Material;
	struct Light;

	class Lighting
	{
	public:
		void init(int shaderProgram);
		void setLightPos(const float* lightPos);
		void setCameraPos(const float* pos);
		void setMaterial(const Material& material);
		void setLight(const Light& light);

	private:
		void setLightAmbient(const float* ambient);
		void setLightDiffuse(const float* diffuse);
		void setLightSpecular(const float* specular);
		void setMaterialAmbient(const float* ambient);
		void setMaterialDiffuse(const float* diffuse);
		void setMaterialSpecular(const float* specular);
		void setMaterialShininess(float shininess);

		void initUniformLocations();
		void setupSettings();

		int getUniformLocation(const char* name) const;

	private:
		int mShaderProgram;
		int mLightPos;
		int mCameraPos;
		int mLightAmbient;
		int mLightDiffuse;
		int mLightSpecular;
		int mMaterialAmbient;
		int mMaterialDiffuse;
		int mMaterialSpecular;
		int mMaterialShininess;
	};
}

