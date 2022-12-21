#pragma once

#include <string>
#include <vector>

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
		__declspec(dllexport) BasicRenderer() noexcept;
		__declspec(dllexport) virtual ~BasicRenderer() noexcept;

		__declspec(dllexport) virtual void init() noexcept;

		__declspec(dllexport) virtual void beginUse() noexcept;
		__declspec(dllexport) virtual void endUse() noexcept;

		__declspec(dllexport) void setMVP(const float* mvp) noexcept;

		__declspec(dllexport) static void printOpenGLErrorMessage() noexcept;

	protected:
		__declspec(dllexport) virtual int loadShader(const std::string& filePath, int shaderType) noexcept;

		__declspec(dllexport) virtual std::vector<char> getShaderLog(int shaderOrProgramId, SHADER_LOG_TYPE logType) noexcept;

		int mVertexShader;
		int mFragmentShader;
		int mShaderProgram;
		int mMVP;
	};
}

