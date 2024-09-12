#pragma once

namespace RenderSystem
{
	struct Material;
	struct Light;

	class SceneLighting
	{
	public:
		void init(int shaderProgram);
		void setLightPos(const float* lightPos) const;
		void setCameraPos(const float* pos) const;
		void setMaterial(const Material& material);
		void setLight(const Light& light);

	private:
		void setLightAmbient(const float* ambient) const;
		void setLightDiffuse(const float* diffuse) const;
		void setLightSpecular(const float* specular) const;
		void setMaterialAmbient(const float* ambient) const;
		void setMaterialDiffuse(const float* diffuse) const;
		void setMaterialSpecular(const float* specular) const;
		void setMaterialShininess(float shininess) const;

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

