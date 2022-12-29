#include "pch.h"

#include "Application.h"

namespace MeshEditor
{
	Application::Application(RenderSystem::Window* window) noexcept	:
		mWindow(window)
	{}

	void Application::start() noexcept
	{
		mWindow->start();
	}
}
