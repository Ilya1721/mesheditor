#pragma once

#include <string>
#include <vector>

#include "MeshCore/ShaderType.h"
#include "BaseUtility/SolutionMacros.h"

namespace RenderSystem
{
	enum class SHADER_LOG_TYPE
	{
		SHADER,
		SHADER_PROGRAM
	};

	class BasicRenderer
	{
	public:
		API BasicRenderer() noexcept;
		API virtual ~BasicRenderer() noexcept;

		API virtual void init() noexcept;

		API virtual int getBasicVertexShader() const noexcept;
		API virtual int getBasicFragmentShader() const noexcept;

		API virtual void beginUse() noexcept;
		API virtual void endUse() noexcept;

		API void setMVP(const float* mvp) noexcept;

		API static void printOpenGLErrorMessage() noexcept;

	protected:
		API virtual int loadShader(const std::string& filePath, int shaderType) noexcept;

		API virtual std::vector<char> getShaderLog(int shaderOrProgramId, SHADER_LOG_TYPE logType) noexcept;

		int mVertexShader;
		int mFragmentShader;
		int mShaderProgram;

		int mMVP;

		MeshCore::ShaderType mActiveShader;
	};
}

