#pragma once

#include "Window.h"

namespace RenderSystem
{
	class MainWindow : public Window
	{
	public:
		__declspec(dllexport) MainWindow(const std::string& title, int width, int height) noexcept;
	};
}

