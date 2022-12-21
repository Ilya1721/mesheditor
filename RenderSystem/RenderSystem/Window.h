#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glfw/glfw3.h>

#include "MeshCore/Mesh.h"
#include "Viewport.h"
#include "Scene.h"

namespace RenderSystem
{
	class Window
	{
	public:
		__declspec(dllexport) Window(const std::string& title, int width, int height) noexcept;
		__declspec(dllexport) Window(const Window& other) = delete;
		__declspec(dllexport) Window(Window&& other) = delete;
		__declspec(dllexport) Window& operator=(const Window& other) = delete;
		__declspec(dllexport) Window& operator=(Window&& other) = delete;

		__declspec(dllexport) virtual ~Window() noexcept = default;

		__declspec(dllexport) virtual void setScene(std::unique_ptr<Scene> scene) noexcept;

		__declspec(dllexport) virtual void start() noexcept;

	private:
		std::string mTitle;

		int mWidth;
		int mHeight;

		GLFWwindow* mWindow;

		std::unique_ptr<Scene> mScene;
	};
}

